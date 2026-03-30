/* NAME:
 *		bvh.h
 *
 * DESCRIPTION:
 *
 * CONTRIBUTORS:
 *		Racc 28-MAR-26
*/





#ifndef BVH_H
#define BVH_H





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "hittable.hpp"
#include "vec.hpp"





namespace RTIW
{




	struct AABB
	{
		vec3 mMin;
		vec3 mMax;

		vec3 Centroid() const noexcept;
		vec3 Extent() const noexcept;
		int LongestAxis() const noexcept;
	};





	struct BVHBinaryNode
	{
		AABB mBoundingBox;

		std::unique_ptr<BVHBinaryNode> mLeftNode;
		std::unique_ptr<BVHBinaryNode> mRightNode;

		const Primitive* Primitive = nullptr;

		bool IsLeaf() const noexcept;
	};





	AABB CombineAABB(const AABB& a, const AABB& b);





	AABB ComputeBounds(const std::vector<const Primitive*>& primitivesList,
	                   const std::size_t start,
	                   const std::size_t end);





	bool HitAABB(const AABB& box, const ray& ray, interval interval);





	std::unique_ptr<BVHBinaryNode> BuildBinaryBVH_MedianSplit(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end);




	bool HitBVH_BinaryTree(const BVHBinaryNode* node,
	                       const ray& ray,
	                       interval interval,
	                       hit_record& hitRecord);





}





#endif // BVH_H





