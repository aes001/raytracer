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





bool RTIW::HitAABB(const AABB& box, const ray& ray, interval interval)
{
	for (int axis = 0; axis < 3; axis++)
	{
		double inverseDirection = 1.0 / ray.direction()[axis];
		double t0 = (box.mMin[axis] - ray.origin()[axis]) * inverseDirection;
		double t1 = (box.mMax[axis] - ray.origin()[axis]) * inverseDirection;

		if (inverseDirection < 0.0)
		{
			std::swap(t0, t1);
		}

		interval.min = std::max(interval.min, t0);
		interval.max = std::min(interval.max, t1);

		if (interval.size() <= 0.0)
		{
			return false;
		}
	}

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





std::unique_ptr<BVHBinaryNode> RTIW::BuildBinaryBVH_MedianSplit(
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

	node->mLeftNode = BuildBinaryBVH_MedianSplit(primitivesList, start, mid);
	node->mRightNode = BuildBinaryBVH_MedianSplit(primitivesList, mid, end);

	return node;
}





bool RTIW::HitBVH_BinaryTree(const BVHBinaryNode* node,
                             const ray& ray,
                             interval interval,
                             hit_record& hitRecord)
{
	if (!node)
	{
		return false;
	}


	if (!HitAABB(node->mBoundingBox, ray, interval))
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
		hitLeft = HitBVH_BinaryTree(node->mLeftNode.get(), ray, interval, leftHitRecord);
	}

	if (hitLeft)
	{
		interval.max = leftHitRecord.t;
	}

	if (node->mRightNode)
	{
		hitRight = HitBVH_BinaryTree(node->mRightNode.get(), ray, interval, rightHitRecord);
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






