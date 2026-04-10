/* NAME:
 *		bvh.hpp
 *
 * DESCRIPTION:
 *		Header for bvh.cpp
 *		An implementation of multiple BVH algorithms for the render.
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

// Standard Library
#include <optional>





namespace RTIW
{




	struct AABB
	{
		vec3 mMin = vec3(+infinity, +infinity, +infinity);
		vec3 mMax = vec3(-infinity, -infinity, -infinity);

		vec3 Centroid() const noexcept;
		vec3 Extent() const noexcept;
		int LongestAxis() const noexcept;

		double SurfaceArea() const noexcept;
	};


	AABB CombineAABB(const AABB& a, const AABB& b);

	AABB ExpandAABBByPoint(const AABB& a, const vec3& point);

	AABB ComputeBounds(const std::vector<const Primitive*>& primitivesList,
	                   const std::size_t start,
	                   const std::size_t end);

	AABB ComputeCentroidBounds(const std::vector<const Primitive *>& primitivesList,
	                           std::size_t start,
	                           std::size_t end);

	bool HitAABB(const AABB& box, const ray& ray, interval& interval);

	bool HitAABB(const AABB& box, const ray& ray, interval interval, hit_record& hr);





	struct BVH2Node
	{
		AABB mBoundingBox;

		std::unique_ptr<BVH2Node> mLeftNode;
		std::unique_ptr<BVH2Node> mRightNode;

		const Primitive* Primitive = nullptr;

		bool IsLeaf() const noexcept;
	};


	std::unique_ptr<BVH2Node> BuildBVH2_MedianSplit(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end);


	bool HitBVH2(const BVH2Node* node,
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





	struct BVH2Node_VariableChild
	{
		AABB mBoundingBox;

		std::unique_ptr<BVH2Node_VariableChild> mLeftNode;
		std::unique_ptr<BVH2Node_VariableChild> mRightNode;

		std::vector<const Primitive*> mChildPrimitives;

		bool IsLeaf() const noexcept;
	};





	std::unique_ptr<BVH2Node_VariableChild> BuildBVH2_SAH_Naive(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end,
		std::optional<AABB> preComputedBB
		);

	bool HitBVH2_VariableChild(const BVH2Node_VariableChild* node,
	                           const ray& ray,
	                           interval interval,
	                           hit_record& hitRecord);





	std::unique_ptr<BVH2Node> BuildBVH2_BottomUp_Naive(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end
		);





}





#endif // BVH_H





