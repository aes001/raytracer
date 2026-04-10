/* NAME:
 *		benchmarkWrappedFunction.h
 *
 * DESCRIPTION:
 *		Wrapper functions to benchmark the actual function calls
 *
 * CONTRIBUTORS:
 *		Racc 06-APR-26
*/





#ifndef BENCHMARK_WRAPPED_FUNCTIONS_HPP
#define BENCHMARK_WRAPPED_FUNCTIONS_HPP





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "bvh.hpp"

// Standard Library






namespace RTIW
{





	std::unique_ptr<BVH2Node> BuildBVH2_MedianSplit_BMWrapper(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end
	);





	std::unique_ptr<BVH4Node> BuildBVH4_EvenSplit_BMWrapper(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end
	);





	std::unique_ptr<BVH8Node> BuildBVH8_EvenSplit_BMWrapper(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end
	);





	std::unique_ptr<BVH2Node> BuildBVH2_BottomUp_Naive_BMWrapper(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end
	);





	std::unique_ptr<BVH2Node_VariableChild> BuildBVH2_SAH_Naive_BMWrapper(
		std::vector<const Primitive*>& primitivesList,
		const std::size_t start,
		const std::size_t end
	);





	std::size_t CountBVHDepth(const BVH2Node* tree);
	std::size_t CountBVHDepth(const BVH4Node* tree);
	std::size_t CountBVHDepth(const BVH8Node* tree);
	std::size_t CountBVHDepth(const BVH2Node_VariableChild* tree);

	std::size_t CountBVHNodes(const BVH2Node* tree);
	std::size_t CountBVHNodes(const BVH4Node* tree);
	std::size_t CountBVHNodes(const BVH8Node* tree);
	std::size_t CountBVHNodes(const BVH2Node_VariableChild* tree);

	std::size_t CountBVHLeafNodes(const BVH2Node* tree);

	std::size_t CountBVHLeafNodesAndCollectLeafSizes(const BVH4Node* tree);
	std::size_t CountBVHLeafNodesAndCollectLeafSizes(const BVH8Node* tree);
	std::size_t CountBVHLeafNodesAndCollectLeafSizes(const BVH2Node_VariableChild* tree);





	struct BVHPerRayStats
	{
		std::int64_t mTraversalDepth = -1;
		std::int64_t mNumberOfNodesVisited = 0;
		std::int64_t mNumberOfAABBTests = 0;
		std::int64_t mNumberOfPrimitiveTests = 0;
	};





	bool HitBVH2_BM(const BVH2Node* node,
	                const ray& ray,
	                interval interval,
	                hit_record& hitRecord,
	                int currentDepth,
	                BVHPerRayStats& stats
	);





	bool HitBVH4_BM(const BVH4Node* node,
	                const ray& ray,
	                interval validTimeInterval,
	                hit_record& hitRecord,
	                int currentDepth,
	                BVHPerRayStats& stats
	);





	bool HitBVH8_BM(const BVH8Node* node,
	                const ray& ray,
	                interval validTimeInterval,
	                hit_record& hitRecord,
	                int currentDepth,
	                BVHPerRayStats& stats
	);





	bool HitBVH2_VariableChild_BM(const BVH2Node_VariableChild* node,
	                              const ray& ray,
	                              interval validTimeInterval,
	                              hit_record& hitRecord,
	                              int currentDepth,
	                              BVHPerRayStats& stats);




}





#endif // BENCHMARK_WRAPPED_FUNCTIONS_HPP
