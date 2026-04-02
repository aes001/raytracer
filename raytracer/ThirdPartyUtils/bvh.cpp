/* NAME:
 *		bvh.cpp
 *
 * DESCRIPTION:
 *		Implementation for BVH generation algorithms
 *
 * CONTRIBUTORS:
 *		Racc 28-MAR-26
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "bvh.hpp"
#include "../convenientNumerics.hpp"
#include "RaccDebug.hpp"
#include "hittable.hpp"

// Standard Library
#include <algorithm>





using namespace RTIW;





vec3 AABB::Centroid() const noexcept
{
	return vec3{(mMin.x() + mMax.x()) / 2.0,
	            (mMin.y() + mMax.y()) / 2.0,
	            (mMin.z() + mMax.z()) / 2.0};
}





vec3 AABB::Extent() const noexcept
{
	return mMax - mMin;
}





int AABB::LongestAxis() const noexcept
{
	vec3 extent = Extent();

	int longestAxis = 0;
	for (int i = 1; i < 3; i++)
	{
		if (extent[longestAxis] < extent[i])
		{
			longestAxis = i;
		}
	}

	return longestAxis;
}





bool BVHBinaryNode::IsLeaf() const noexcept
{
	return Primitive != nullptr;
}





AABB RTIW::CombineAABB(const AABB& a, const AABB& b)
{
	return AABB{
		vec3{std::min(a.mMin.x(), b.mMin.x()),
		     std::min(a.mMin.y(), b.mMin.y()),
		     std::min(a.mMin.z(), b.mMin.z())},

		vec3{std::max(a.mMax.x(), b.mMax.x()),
		     std::max(a.mMax.y(), b.mMax.y()),
		     std::max(a.mMax.z(), b.mMax.z())}

	};
}





bool RTIW::HitAABB(const AABB& box, const ray& ray, interval& validTimeRange)
{

	interval tempValidTimeRange = validTimeRange;

	for (int axis = 0; axis < 3; axis++)
	{
		double inverseDirection = 1.0 / ray.direction()[axis];
		double t0 = (box.mMin[axis] - ray.origin()[axis]) * inverseDirection;
		double t1 = (box.mMax[axis] - ray.origin()[axis]) * inverseDirection;

		if (inverseDirection < 0.0)
		{
			std::swap(t0, t1);
		}

		tempValidTimeRange.min = std::max(tempValidTimeRange.min, t0);
		tempValidTimeRange.max = std::min(tempValidTimeRange.max, t1);

		if (tempValidTimeRange.size() <= 0.0)
		{
			return false;
		}
	}

	validTimeRange = tempValidTimeRange;
	return true;
}





AABB RTIW::ComputeBounds(const std::vector<const Primitive *>& primitivesList,
                         std::size_t start,
                         std::size_t end)
{
	AABB bounds = primitivesList[start]->GetBoundingBox();

	for (auto it = primitivesList.begin() + start + 1;
	     it != primitivesList.begin() + end;
	     it++)
	{
		bounds = CombineAABB(bounds, (*it)->GetBoundingBox());
	}

	return bounds;
}





std::unique_ptr<BVHBinaryNode> RTIW::BuildBVH2_MedianSplit(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	auto node = std::make_unique<BVHBinaryNode>();

	node->mBoundingBox = ComputeBounds(primitivesList, start, end);

	const std::size_t primitiveCount = end - start;

	if (primitiveCount == 1)
	{
		node->Primitive = primitivesList[start];
		return node;
	}

	const std::size_t longestAxis = node->mBoundingBox.LongestAxis();

	const auto sortPrimitivesByAxisFunc = [longestAxis]
		(const Primitive* a, const Primitive* b)
		{
			return a->GetBoundingBox().Centroid()[longestAxis] <
				   b->GetBoundingBox().Centroid()[longestAxis];
		};

	std::sort(primitivesList.begin() + start,
	          primitivesList.begin() + end,
	          sortPrimitivesByAxisFunc);

	const std::size_t mid = start + (primitiveCount / 2);

	node->mLeftNode = BuildBVH2_MedianSplit(primitivesList, start, mid);
	node->mRightNode = BuildBVH2_MedianSplit(primitivesList, mid, end);

	return node;
}





bool RTIW::HitBVH2(const BVHBinaryNode* node,
                   const ray& ray,
                   interval interval,
                   hit_record& hitRecord)
{
	if (!node)
	{
		return false;
	}


	if (!HitAABB(node->mBoundingBox, ray, interval)) // Will resize the interval to the BB enter and exit
	{
		return false;
	}


	if (node->IsLeaf())
	{
		return node->Primitive->hit(ray, interval, hitRecord);
	}


	hit_record leftHitRecord;
	hit_record rightHitRecord;

	bool hitLeft = false;
	bool hitRight = false;

	if (node->mLeftNode)
	{
		hitLeft = HitBVH2(node->mLeftNode.get(), ray, interval, leftHitRecord);
	}

	if (hitLeft)
	{
		interval.max = leftHitRecord.t;
	}

	if (node->mRightNode)
	{
		hitRight = HitBVH2(node->mRightNode.get(), ray, interval, rightHitRecord);
	}

	if (hitLeft && hitRight)
	{
		hitRecord = (leftHitRecord.t < rightHitRecord.t) ? leftHitRecord : rightHitRecord;
		return true;
	}

	if (hitLeft)
	{
		hitRecord = leftHitRecord;
		return true;
	}

	if (hitRight)
	{
		hitRecord = rightHitRecord;
		return true;
	}


	return false;
}





bool BVH4Node::IsLeaf() const noexcept
{
	return mPrimitiveCount != 0;
}





std::unique_ptr<BVH4Node> RTIW::BuildBVH4_EvenSplit(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	auto node = std::make_unique<BVH4Node>();

	const std::size_t primitiveCount = end - start;

	if (primitiveCount <= 4)
	{
		for (std::size_t i = 0; i < primitiveCount; i++)
		{
			node->mChildPrimitives[i] = primitivesList[start + i];
			node->mChildBoundingBoxes[i] = primitivesList[start + i]->GetBoundingBox();
		}
		node->mPrimitiveCount = primitiveCount;
		return node;
	}

	AABB nodeBB = ComputeBounds(primitivesList, start, end);

	const std::size_t longestAxis = nodeBB.LongestAxis();

	const auto sortPrimitivesByAxisFunc = [longestAxis]
		(const Primitive* a, const Primitive* b)
		{
			return a->GetBoundingBox().Centroid()[longestAxis] <
				   b->GetBoundingBox().Centroid()[longestAxis];
		};

	std::sort(primitivesList.begin() + start,
	          primitivesList.begin() + end,
	          sortPrimitivesByAxisFunc);

	const std::size_t count1 = primitiveCount / 4 + (primitiveCount % 4 > 0 ? 1 : 0);
	const std::size_t count2 = primitiveCount / 4 + (primitiveCount % 4 > 1 ? 1 : 0);
	const std::size_t count3 = primitiveCount / 4 + (primitiveCount % 4 > 2 ? 1 : 0);

	const std::size_t d1 = start + count1;
	const std::size_t d2 = d1 + count2;
	const std::size_t d3 = d2 + count3;

	node->mChildNodes[0] = BuildBVH4_EvenSplit(primitivesList, start, d1);
	node->mChildNodes[1] = BuildBVH4_EvenSplit(primitivesList, d1,    d2);
	node->mChildNodes[2] = BuildBVH4_EvenSplit(primitivesList, d2,    d3);
	node->mChildNodes[3] = BuildBVH4_EvenSplit(primitivesList, d3,    end);

	node->mChildBoundingBoxes[0] = ComputeBounds(primitivesList, start, d1);
	node->mChildBoundingBoxes[1] = ComputeBounds(primitivesList, d1,    d2);
	node->mChildBoundingBoxes[2] = ComputeBounds(primitivesList, d2,    d3);
	node->mChildBoundingBoxes[3] = ComputeBounds(primitivesList, d3,    end);

	node->mChildNodeCount = 4;

	return node;
}





struct BBHitInfo
{
	std::size_t mIndex;
	interval mBBHitInterval;
};





bool RTIW::HitBVH4(const BVH4Node* node,
                   const ray& ray,
                   interval validTimeInterval,
                   hit_record& hitRecord)
{
	if (!node)
	{
		return false;
	}


	if (node->IsLeaf())
	{
		hit_record tempRec;
		bool hitAnything = false;
		double closestSoFar = validTimeInterval.max;

		for (std::size_t i = 0; i < node->mPrimitiveCount; i++)
		{
			if (node->mChildPrimitives[i]->hit(ray, interval(validTimeInterval.min, closestSoFar), tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.t;
				hitRecord = tempRec;
			}
		}

		return hitAnything;
	}


	BBHitInfo bbHitInfoList[4];
	std::size_t bbHitCount = 0;

	for (std::size_t i = 0; i < 4; i++)
	{
		interval tempInterval = validTimeInterval;
		if (HitAABB(node->mChildBoundingBoxes[i], ray, tempInterval))
		{
			bbHitInfoList[bbHitCount] = {i, tempInterval};
			bbHitCount++;
		}
	}


	// Visit the node with the closest bounding box first
	std::sort(bbHitInfoList, bbHitInfoList + bbHitCount,
		[] (const BBHitInfo& a, const BBHitInfo& b)
		{
			return a.mBBHitInterval.min < b.mBBHitInterval.min;
		}
	);

	double closestPrimHitTSoFar = validTimeInterval.max;
	hit_record tempPrimHitRecord;
	bool hit = false;
	for (std::size_t i = 0; i < bbHitCount; i++)
	{
		const std::size_t currentChildIndex = bbHitInfoList[i].mIndex;
		if (bbHitInfoList[i].mBBHitInterval.min >= closestPrimHitTSoFar)
		{
			// Bounding Box entry time is after a primitive hit. Skip.
			continue;
		}

		if(HitBVH4(node->mChildNodes[currentChildIndex].get(),
		           ray,
		           interval(validTimeInterval.min, closestPrimHitTSoFar),
		           tempPrimHitRecord))
		{
			hit = true;
			closestPrimHitTSoFar = tempPrimHitRecord.t;
			hitRecord = tempPrimHitRecord;
		}
	}

	return hit;
}




bool BVH8Node::IsLeaf() const noexcept
{
	return mPrimitiveCount != 0;
}





std::unique_ptr<BVH8Node> RTIW::BuildBVH8_EvenSplit(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	auto node = std::make_unique<BVH8Node>();

	const std::size_t primitiveCount = end - start;

	if (primitiveCount <= 8)
	{
		for (std::size_t i = 0; i < primitiveCount; i++)
		{
			node->mChildPrimitives[i]    = primitivesList[start + i];
			node->mChildBoundingBoxes[i] =
				primitivesList[start + i]->GetBoundingBox();
		}
		node->mPrimitiveCount = primitiveCount;
		return node;
	}

	AABB nodeBB = ComputeBounds(primitivesList, start, end);

	const std::size_t longestAxis = nodeBB.LongestAxis();

	const auto sortPrimitivesByAxisFunc = [longestAxis]
		(const Primitive* a, const Primitive* b)
		{
			return a->GetBoundingBox().Centroid()[longestAxis] <
			       b->GetBoundingBox().Centroid()[longestAxis];
		};

	std::sort(primitivesList.begin() + start,
	          primitivesList.begin() + end,
	          sortPrimitivesByAxisFunc);

	const std::size_t evenSplit = primitiveCount / 8;
	std::size_t remainder = primitiveCount % 8;

	std::size_t primitiveDivisionStartIndex = start;

	for (std::size_t i = 0; i < 8; i++)
	{
		std::size_t primitiveDivisionEndIndex = primitiveDivisionStartIndex +
		                                        evenSplit;
		if (remainder > 0)
		{
			primitiveDivisionEndIndex++;
			remainder--;
		}

		node->mChildNodes[i] = BuildBVH8_EvenSplit(primitivesList,
		                                           primitiveDivisionStartIndex,
		                                           primitiveDivisionEndIndex);

		node->mChildBoundingBoxes[i] = ComputeBounds(primitivesList,
		                                             primitiveDivisionStartIndex,
		                                             primitiveDivisionEndIndex);

		primitiveDivisionStartIndex = primitiveDivisionEndIndex;
	}

	node->mChildNodeCount = 8;

	return node;
}





bool RTIW::HitBVH8(const BVH8Node* node,
                   const ray& ray,
                   interval validTimeInterval,
                   hit_record& hitRecord)
{
	if (!node)
	{
		return false;
	}


	if (node->IsLeaf())
	{
		hit_record tempRec;
		bool hitAnything = false;
		double closestSoFar = validTimeInterval.max;

		for (std::size_t i = 0; i < node->mPrimitiveCount; i++)
		{
			if (node->mChildPrimitives[i]->hit(ray, interval(validTimeInterval.min, closestSoFar), tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.t;
				hitRecord = tempRec;
			}
		}

		return hitAnything;
	}


	BBHitInfo bbHitInfoList[8];
	std::size_t bbHitCount = 0;

	for (std::size_t i = 0; i < 8; i++)
	{
		interval tempInterval = validTimeInterval;
		if (HitAABB(node->mChildBoundingBoxes[i], ray, tempInterval))
		{
			bbHitInfoList[bbHitCount] = {i, tempInterval};
			bbHitCount++;
		}
	}


	// Visit the node with the closest bounding box first
	std::sort(bbHitInfoList, bbHitInfoList + bbHitCount,
		[] (const BBHitInfo& a, const BBHitInfo& b)
		{
			return a.mBBHitInterval.min < b.mBBHitInterval.min;
		}
	);

	double closestPrimHitTSoFar = validTimeInterval.max;
	hit_record tempPrimHitRecord;
	bool hit = false;
	for (std::size_t i = 0; i < bbHitCount; i++)
	{
		const std::size_t currentChildIndex = bbHitInfoList[i].mIndex;
		if (bbHitInfoList[i].mBBHitInterval.min >= closestPrimHitTSoFar)
		{
			// Bounding Box entry time is after a primitive hit. Skip.
			continue;
		}

		if(HitBVH8(node->mChildNodes[currentChildIndex].get(),
		           ray,
		           interval(validTimeInterval.min, closestPrimHitTSoFar),
		           tempPrimHitRecord))
		{
			hit = true;
			closestPrimHitTSoFar = tempPrimHitRecord.t;
			hitRecord = tempPrimHitRecord;
		}
	}

	return hit;
}





