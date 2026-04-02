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


	AABB CombineAABB(const AABB& a, const AABB& b);

	AABB ComputeBounds(const std::vector<const Primitive*>& primitivesList,
	                   const std::size_t start,
	                   const std::size_t end);

	bool HitAABB(const AABB& box, const ray& ray, interval& interval);

	bool HitAABB(const AABB& box, const ray& ray, interval interval, hit_record& hr);





	struct BVHBinaryNode
	{
		AABB mBoundingBox;

		std::unique_ptr<BVHBinaryNode> mLeftNode;
		std::unique_ptr<BVHBinaryNode> mRightNode;

		const Primitive* Primitive = nullptr;

		bool IsLeaf() const noexcept;
	};


	std::unique_ptr<BVHBinaryNode> BuildBVH2_MedianSplit(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end);


	bool HitBVH2(const BVHBinaryNode* node,
	                       const ray& ray,
	                       interval interval,
	                       hit_record& hitRecord);





	struct BVH4Node
	{
		AABB mChildBoundingBoxes[4];

		std::unique_ptr<BVH4Node> mChildNodes[4];
		std::size_t mChildNodeCount = 0;

		// To do: Maybe use a weak pointer??
		const Primitive* mChildPrimitives[4];
		std::size_t mPrimitiveCount = 0;

		bool IsLeaf() const noexcept;
	};

	std::unique_ptr<BVH4Node> BuildBVH4_EvenSplit(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end);

	bool HitBVH4(const BVH4Node* node,
	             const ray& ray,
	             interval interval,
	             hit_record& hitRecord);





	struct BVH8Node
	{
		AABB mChildBoundingBoxes[8];

		std::unique_ptr<BVH8Node> mChildNodes[8];
		std::size_t mChildNodeCount = 0;

		// To do: Maybe use a weak pointer??
		const Primitive* mChildPrimitives[8];
		std::size_t mPrimitiveCount = 0;

		bool IsLeaf() const noexcept;
	};

	std::unique_ptr<BVH8Node> BuildBVH8_EvenSplit(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end);

	bool HitBVH8(const BVH8Node* node,
	             const ray& ray,
	             interval interval,
	             hit_record& hitRecord);






}





#endif // BVH_H





