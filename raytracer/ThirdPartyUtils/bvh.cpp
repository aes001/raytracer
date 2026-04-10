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
#include "hittable.hpp"
#include "../instrumenter.hpp"
#include "RaccDebug.hpp"

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





double AABB::SurfaceArea() const noexcept
{
	double dx = mMax.x() - mMin.x();
	double dy = mMax.y() - mMin.y();
	double dz = mMax.z() - mMin.z();

	RACC_REQUIRE(dx >= 0.0 || dy >= 0.0 || dz >= 0.0,
	             "Error: Invalid bounding box, min and max might be switched");

	return 2.0 * ((dx * dy) + (dx * dz) + (dy * dz));
}





bool BVH2Node::IsLeaf() const noexcept
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





AABB RTIW::ExpandAABBByPoint(const AABB& a, const vec3& point)
{
	return AABB{
		vec3{std::min(a.mMin.x(), point.x()),
		     std::min(a.mMin.y(), point.y()),
		     std::min(a.mMin.z(), point.z())},

		vec3{std::max(a.mMax.x(), point.x()),
		     std::max(a.mMax.y(), point.y()),
		     std::max(a.mMax.z(), point.z())}
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
	     ++it)
	{
		bounds = CombineAABB(bounds, (*it)->GetBoundingBox());
	}

	return bounds;
}





AABB RTIW::ComputeCentroidBounds(const std::vector<const Primitive *>& primitivesList,
                                 std::size_t start,
                                 std::size_t end)
{
	AABB centroidBounds;

	for (auto it = primitivesList.begin() + start;
	     it != primitivesList.begin() + end;
	     ++it)
	{
		centroidBounds = ExpandAABBByPoint(centroidBounds, (*it)->GetBoundingBox().Centroid());
	}

	return centroidBounds;
}





std::unique_ptr<BVH2Node> RTIW::BuildBVH2_MedianSplit(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	auto node = std::make_unique<BVH2Node>();

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





bool RTIW::HitBVH2(const BVH2Node* node,
                   const ray& ray,
                   interval interval,
                   hit_record& hitRecord)
{
	if (!node)
	{
		return false;
	}


	if (node->IsLeaf())
	{
		return node->Primitive->hit(ray, interval, hitRecord);
	}


	if (!HitAABB(node->mBoundingBox, ray, interval)) // Will resize the interval to the BB enter and exit
	{
		return false;
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





bool BVH2Node_VariableChild::IsLeaf() const noexcept
{
	return (mLeftNode == nullptr) && (mRightNode == nullptr);
}





std::unique_ptr<BVH2Node_VariableChild> RTIW::BuildBVH2_SAH_Naive(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end,
	std::optional<AABB> preComputedBB)
{
	auto node = std::make_unique<BVH2Node_VariableChild>();

	if (preComputedBB.has_value())
	{
		node->mBoundingBox = preComputedBB.value();
	}
	else
	{
		node->mBoundingBox = ComputeBounds(primitivesList, start, end);
	}

	const std::size_t primitiveCount = end - start;

	if (primitiveCount == 1)
	{
		node->mChildPrimitives.push_back(primitivesList[start]);
		return node;
	}

	const auto GenerateSortFunc = []
		(std::size_t index)
		{
			return [index] (const Primitive* a, const Primitive* b)
				{
					return a->GetBoundingBox().Centroid()[index] <
					       b->GetBoundingBox().Centroid()[index];
				};
		};

	double cheapestSplitCost = +infinity;
	std::size_t cheapestSplitIndex = static_cast<std::size_t>(start + primitiveCount / 2);
	std::size_t idealAxis = 0;
	AABB leftIdealBB;
	AABB rightIdealBB;

	for (std::size_t axis = 0; axis < 3; ++axis)
	{
		std::sort(primitivesList.begin() + start,
		          primitivesList.begin() + end,
		          GenerateSortFunc(axis));

		std::vector<AABB> prefix(primitiveCount);
		std::vector<AABB> suffix(primitiveCount);

		AABB prefixMaster;
		AABB suffixMaster;
		for (std::size_t i = 0; i < primitiveCount; ++i)
		{
			prefixMaster = CombineAABB(prefixMaster, primitivesList[i + start]->GetBoundingBox());
			prefix[i] = prefixMaster;

			suffixMaster = CombineAABB(suffixMaster, primitivesList[(end - 1) - i]->GetBoundingBox());
			suffix[primitiveCount - 1 - i] = suffixMaster;
		}

		for (std::size_t i = 0; i < primitiveCount - 1; ++i)
		{
			AABB leftBB  = prefix[i];
			AABB rightBB = suffix[i + 1];

			const double leftPrimitiveCount = i + 1;
			const double rightPrimitiveCount = primitiveCount - (1 + i);

			const double leftCost  =
				(leftBB.SurfaceArea() / node->mBoundingBox.SurfaceArea()) *
				(leftPrimitiveCount);
			const double rightCost =
				(rightBB.SurfaceArea() / node->mBoundingBox.SurfaceArea()) *
				(rightPrimitiveCount);

			const double totalCost = leftCost + rightCost + 1;

			if (totalCost < cheapestSplitCost)
			{
				cheapestSplitCost = totalCost;
				cheapestSplitIndex = start + 1 + i;

				leftIdealBB = leftBB;
				rightIdealBB = rightBB;
				idealAxis = axis;
			}
		}
	}


	// Check how much it costs if we build a leaf instead
	const double leafCost = static_cast<double>(primitiveCount);

	const bool cheaperToMakeLeaf = leafCost <= cheapestSplitCost;
	if (cheaperToMakeLeaf)
	{
		for (auto it = primitivesList.begin() + start;
		     it != primitivesList.begin() + end;
		     ++it)
		{
			node->mChildPrimitives.push_back(*it);
		}
	}
	else
	{
		std::sort(primitivesList.begin() + start,
		          primitivesList.begin() + end,
		          GenerateSortFunc(idealAxis));

		node->mLeftNode = BuildBVH2_SAH_Naive(primitivesList, start, cheapestSplitIndex, leftIdealBB);
		node->mRightNode = BuildBVH2_SAH_Naive(primitivesList, cheapestSplitIndex, end, rightIdealBB);
	}

	return node;
}





bool RTIW::HitBVH2_VariableChild(const BVH2Node_VariableChild* node,
                                 const ray& ray,
                                 interval validTimeInterval,
                                 hit_record& hitRecord)
{
	if (!node)
	{
		return false;
	}


	if (!HitAABB(node->mBoundingBox, ray, validTimeInterval)) // Will resize the interval to the BB enter and exit
	{
		return false;
	}


	if (node->IsLeaf())
	{
		hit_record tempRec;
		bool hitAnything = false;
		double closestSoFar = validTimeInterval.max;

		for (const auto* primitive : node->mChildPrimitives)
		{
			if (primitive->hit(ray, interval(validTimeInterval.min, closestSoFar), tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.t;
				hitRecord = tempRec;
			}
		}

		return hitAnything;
	}


	hit_record leftHitRecord;
	hit_record rightHitRecord;

	bool hitLeft = false;
	bool hitRight = false;

	if (node->mLeftNode)
	{
		hitLeft = HitBVH2_VariableChild(node->mLeftNode.get(), ray, validTimeInterval, leftHitRecord);
	}

	if (hitLeft)
	{
		validTimeInterval.max = leftHitRecord.t;
	}

	if (node->mRightNode)
	{
		hitRight = HitBVH2_VariableChild(node->mRightNode.get(), ray, validTimeInterval, rightHitRecord);
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





struct BottomUpActiveNode
{
	std::unique_ptr<BVH2Node> mNode;
	std::size_t mChildPrimitivesCount = 1;
};





std::unique_ptr<BVH2Node> RTIW::BuildBVH2_BottomUp_Naive(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	const std::size_t primitiveCount = end - start;

	RACC_REQUIRE(primitiveCount != 0, "Error: trying to create BVH tree with no primitives");

	// Creating leaf nodes first
	std::vector<BottomUpActiveNode> mergableNodes(primitiveCount);
	for (std::size_t i = 0; i < primitiveCount; ++i)
	{
		mergableNodes[i].mNode = std::make_unique<BVH2Node>();
		mergableNodes[i].mNode->Primitive = primitivesList[i + start];
		mergableNodes[i].mNode->mBoundingBox = primitivesList[i + start]->GetBoundingBox();
	}


	while (mergableNodes.size() > 1)
	{
		double cheapestMergeCost = +infinity;
		std::size_t idealLeftNode = 0;
		std::size_t idealRightNode = 0;
		AABB newNodeBB;

		for (std::size_t leftNodeIdx = 0; leftNodeIdx < mergableNodes.size(); ++leftNodeIdx)
		{
			for (std::size_t rightNodeIdx = leftNodeIdx + 1; rightNodeIdx < mergableNodes.size(); ++rightNodeIdx)
			{
				AABB leftBB = mergableNodes[leftNodeIdx].mNode->mBoundingBox;
				AABB rightBB = mergableNodes[rightNodeIdx].mNode->mBoundingBox;

				const double leftPrimitiveCount = mergableNodes[leftNodeIdx].mChildPrimitivesCount;
				const double rightPrimitiveCount = mergableNodes[rightNodeIdx].mChildPrimitivesCount;

				AABB combinedBB = CombineAABB(leftBB, rightBB);
				const double totalCost = combinedBB.SurfaceArea() * (leftPrimitiveCount + rightPrimitiveCount);

				if (totalCost < cheapestMergeCost)
				{
					cheapestMergeCost = totalCost;
					idealLeftNode = leftNodeIdx;
					idealRightNode = rightNodeIdx;
					newNodeBB = combinedBB;
				}
			}
		}

		BottomUpActiveNode newMergedActiveNode;
		newMergedActiveNode.mNode = std::make_unique<BVH2Node>();
		newMergedActiveNode.mNode->mLeftNode = std::move(mergableNodes[idealLeftNode].mNode);
		newMergedActiveNode.mNode->mRightNode = std::move(mergableNodes[idealRightNode].mNode);
		newMergedActiveNode.mNode->mBoundingBox = newNodeBB;
		newMergedActiveNode.mChildPrimitivesCount = mergableNodes[idealLeftNode].mChildPrimitivesCount +
		                                            mergableNodes[idealRightNode].mChildPrimitivesCount;

		std::swap(mergableNodes[idealRightNode], mergableNodes.back());
		mergableNodes.pop_back();

		mergableNodes[idealLeftNode] = std::move(newMergedActiveNode);
	}

	return std::move(mergableNodes[0].mNode);
}





