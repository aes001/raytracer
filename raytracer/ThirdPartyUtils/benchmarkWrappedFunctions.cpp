/* NAME:
 *		benchmarkWrappedFunction.cpp
 *
 * DESCRIPTION:
 *		Wrapper functions to benchmark the actual function calls
 *
 * CONTRIBUTORS:
 *		Racc 06-APR-26
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "benchmarkWrappedFunctions.hpp"
#include "../instrumenter.hpp"

// Standard Library





using namespace RTIW;






std::unique_ptr<BVH2Node> RTIW::BuildBVH2_MedianSplit_BMWrapper(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	Benchmarker* bmPtr = Benchmarker::Get();

	if (BenchmarkerState::kRenderTimingStats == bmPtr->mState)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		auto res = BuildBVH2_MedianSplit(primitivesList, start, end);
		auto endTime = std::chrono::high_resolution_clock::now();

		auto buildTime =
			std::chrono::duration_cast<BMT_BuildTime>(endTime - startTime);

		bmPtr->mOverallTimingStats.back().mBVHBuildTimeResults = buildTime;

		return res;
	}
	else if (BenchmarkerState::kRenderCountingStats == bmPtr->mState)
	{
		auto res = BuildBVH2_MedianSplit(primitivesList, start, end);

		auto& statsContainer = bmPtr->mCountingStats.back();
		statsContainer.mTreeDepth = CountBVHDepth(res.get());
		statsContainer.mTotalNumberOfNodes = CountBVHNodes(res.get());
		statsContainer.mTotalNumberOfLeafNodes = CountBVHLeafNodes(res.get());
		statsContainer.mLeafSizes.Add(1);

		statsContainer.mMemoryUsage =
			(sizeof(BVH2Node) * statsContainer.mTotalNumberOfNodes) +
			(sizeof(const Primitive*) * BuildMetaData::RunTimeBuildInfo::Get()->mPrimitiveCount);

		return res;
	}
	else
	{
		return BuildBVH2_MedianSplit(primitivesList, start, end);
	}
}





std::unique_ptr<BVH4Node> RTIW::BuildBVH4_EvenSplit_BMWrapper(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	Benchmarker* bmPtr = Benchmarker::Get();

	if (BenchmarkerState::kRenderTimingStats == bmPtr->mState)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		auto res = BuildBVH4_EvenSplit(primitivesList, start, end);
		auto endTime = std::chrono::high_resolution_clock::now();

		auto buildTime =
			std::chrono::duration_cast<BMT_BuildTime>(endTime - startTime);

		bmPtr->mOverallTimingStats.back().mBVHBuildTimeResults = buildTime;

		return res;
	}
	else if (BenchmarkerState::kRenderCountingStats == bmPtr->mState)
	{
		auto res = BuildBVH4_EvenSplit(primitivesList, start, end);

		auto& statsContainer = bmPtr->mCountingStats.back();
		statsContainer.mTreeDepth = CountBVHDepth(res.get());
		statsContainer.mTotalNumberOfNodes = CountBVHNodes(res.get());

		statsContainer.mTotalNumberOfLeafNodes =
			CountBVHLeafNodesAndCollectLeafSizes(res.get());

		statsContainer.mMemoryUsage =
			(sizeof(BVH4Node) * statsContainer.mTotalNumberOfNodes) +
			(sizeof(const Primitive*) * BuildMetaData::RunTimeBuildInfo::Get()->mPrimitiveCount);

		return res;
	}
	else
	{
		return BuildBVH4_EvenSplit(primitivesList, start, end);
	}
}





std::unique_ptr<BVH8Node> RTIW::BuildBVH8_EvenSplit_BMWrapper(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	Benchmarker* bmPtr = Benchmarker::Get();

	if (BenchmarkerState::kRenderTimingStats == bmPtr->mState)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		auto res = BuildBVH8_EvenSplit(primitivesList, start, end);
		auto endTime = std::chrono::high_resolution_clock::now();

		auto buildTime =
			std::chrono::duration_cast<BMT_BuildTime>(endTime - startTime);

		bmPtr->mOverallTimingStats.back().mBVHBuildTimeResults = buildTime;

		return res;
	}
	else if (BenchmarkerState::kRenderCountingStats == bmPtr->mState)
	{
		auto res = BuildBVH8_EvenSplit(primitivesList, start, end);
		auto& statsContainer = bmPtr->mCountingStats.back();
		statsContainer.mTreeDepth = CountBVHDepth(res.get());
		statsContainer.mTotalNumberOfNodes = CountBVHNodes(res.get());

		statsContainer.mTotalNumberOfLeafNodes =
			CountBVHLeafNodesAndCollectLeafSizes(res.get());

		statsContainer.mMemoryUsage =
			(sizeof(BVH8Node) * statsContainer.mTotalNumberOfNodes) +
			(sizeof(const Primitive*) * BuildMetaData::RunTimeBuildInfo::Get()->mPrimitiveCount);

		return res;
	}
	else
	{
		return BuildBVH8_EvenSplit(primitivesList, start, end);
	}
}





std::unique_ptr<BVH2Node> RTIW::BuildBVH2_BottomUp_Naive_BMWrapper(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	Benchmarker* bmPtr = Benchmarker::Get();

	if (BenchmarkerState::kRenderTimingStats == bmPtr->mState)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		auto res = BuildBVH2_BottomUp_Naive(primitivesList, start, end);
		auto endTime = std::chrono::high_resolution_clock::now();

		auto buildTime =
			std::chrono::duration_cast<BMT_BuildTime>(endTime - startTime);

		bmPtr->mOverallTimingStats.back().mBVHBuildTimeResults = buildTime;

		return res;
	}
	else if (BenchmarkerState::kRenderCountingStats == bmPtr->mState)
	{
		auto res = BuildBVH2_BottomUp_Naive(primitivesList, start, end);
		auto& statsContainer = bmPtr->mCountingStats.back();
		statsContainer.mTreeDepth = CountBVHDepth(res.get());
		statsContainer.mTotalNumberOfNodes = CountBVHNodes(res.get());

		statsContainer.mTotalNumberOfLeafNodes = CountBVHLeafNodes(res.get());
		statsContainer.mLeafSizes.Add(1);

		statsContainer.mMemoryUsage =
			(sizeof(BVH2Node) * statsContainer.mTotalNumberOfNodes) +
			(sizeof(const Primitive*) * BuildMetaData::RunTimeBuildInfo::Get()->mPrimitiveCount);

		return res;
	}
	else
	{
		return BuildBVH2_BottomUp_Naive(primitivesList, start, end);
	}
}





std::unique_ptr<BVH2Node_VariableChild> RTIW::BuildBVH2_SAH_Naive_BMWrapper(
	std::vector<const Primitive*>& primitivesList,
	const std::size_t start,
	const std::size_t end)
{
	Benchmarker* bmPtr = Benchmarker::Get();

	if (BenchmarkerState::kRenderTimingStats == bmPtr->mState)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		auto res = BuildBVH2_SAH_Naive(primitivesList,
		                               start,
		                               end,
		                               std::nullopt);
		auto endTime = std::chrono::high_resolution_clock::now();

		auto buildTime =
			std::chrono::duration_cast<BMT_BuildTime>(endTime - startTime);

		bmPtr->mOverallTimingStats.back().mBVHBuildTimeResults = buildTime;

		return res;
	}
	else if (BenchmarkerState::kRenderCountingStats == bmPtr->mState)
	{
		auto res = BuildBVH2_SAH_Naive(primitivesList,
		                               start,
		                               end,
		                               std::nullopt);
		auto& statsContainer = bmPtr->mCountingStats.back();
		statsContainer.mTreeDepth = CountBVHDepth(res.get());
		statsContainer.mTotalNumberOfNodes = CountBVHNodes(res.get());

		statsContainer.mTotalNumberOfLeafNodes =
			CountBVHLeafNodesAndCollectLeafSizes(res.get());

		statsContainer.mMemoryUsage =
			(sizeof(BVH2Node_VariableChild)
			* statsContainer.mTotalNumberOfNodes)
			+ (sizeof(const Primitive*) * BuildMetaData::RunTimeBuildInfo::Get()->mPrimitiveCount);

		return res;
	}
	else
	{
		return BuildBVH2_SAH_Naive(primitivesList, start, end, std::nullopt);
	}
}





std::size_t RTIW::CountBVHDepth(const BVH2Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	if (tree->IsLeaf())
	{
		return 1;
	}

	return 1 + std::max(
		CountBVHDepth(tree->mLeftNode.get()),
		CountBVHDepth(tree->mRightNode.get()));
}





std::size_t RTIW::CountBVHDepth(const BVH4Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	if (tree->IsLeaf())
	{
		return 1;
	}

	std::size_t child0Count = CountBVHDepth(tree->mChildNodes[0].get());
	std::size_t child1Count = CountBVHDepth(tree->mChildNodes[1].get());
	std::size_t child2Count = CountBVHDepth(tree->mChildNodes[2].get());
	std::size_t child3Count = CountBVHDepth(tree->mChildNodes[3].get());

	return 1 + std::max({child0Count, child1Count, child2Count, child3Count});
}





std::size_t RTIW::CountBVHDepth(const BVH8Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	if (tree->IsLeaf())
	{
		return 1;
	}

	std::size_t depthCounts[8]{};

	for (std::size_t i = 0; i < 8; ++i)
	{
		depthCounts[i] = CountBVHDepth(tree->mChildNodes[i].get());
	}

	return 1 + *(std::max_element(depthCounts, depthCounts + 8));
}





std::size_t RTIW::CountBVHDepth(const BVH2Node_VariableChild* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	if (tree->IsLeaf())
	{
		return 1;
	}

	return 1 + std::max(
		CountBVHDepth(tree->mLeftNode.get()),
		CountBVHDepth(tree->mRightNode.get()));
}





std::size_t RTIW::CountBVHNodes(const BVH2Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}


	return 1 + CountBVHNodes(tree->mLeftNode.get())
	         + CountBVHNodes(tree->mRightNode.get());
}





std::size_t RTIW::CountBVHNodes(const BVH4Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	std::size_t child0Count = CountBVHNodes(tree->mChildNodes[0].get());
	std::size_t child1Count = CountBVHNodes(tree->mChildNodes[1].get());
	std::size_t child2Count = CountBVHNodes(tree->mChildNodes[2].get());
	std::size_t child3Count = CountBVHNodes(tree->mChildNodes[3].get());

	return 1 + child0Count + child1Count + child2Count + child3Count;
}





std::size_t RTIW::CountBVHNodes(const BVH8Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	std::size_t count = 0;
	for (size_t i = 0; i < 8; ++i)
	{
		count += CountBVHNodes(tree->mChildNodes[i].get());
	}

	return 1 + count;
}





std::size_t RTIW::CountBVHNodes(const BVH2Node_VariableChild* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}


	return 1 + CountBVHNodes(tree->mLeftNode.get())
	         + CountBVHNodes(tree->mRightNode.get());
}





std::size_t RTIW::CountBVHLeafNodes(const BVH2Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	if (tree->IsLeaf())
	{
		return 1;
	}


	return CountBVHLeafNodes(tree->mLeftNode.get())
	       + CountBVHLeafNodes(tree->mRightNode.get());
}





std::size_t RTIW::CountBVHLeafNodesAndCollectLeafSizes(const BVH4Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	if (tree->IsLeaf())
	{
		// What the heck was I thinking?
		auto& leafSizes = Benchmarker::Get()->mCountingStats.back().mLeafSizes;
		leafSizes.Add(tree->mPrimitiveCount);
		return 1;
	}

	std::size_t child0Count =
		CountBVHLeafNodesAndCollectLeafSizes(tree->mChildNodes[0].get());

	std::size_t child1Count =
		CountBVHLeafNodesAndCollectLeafSizes(tree->mChildNodes[1].get());

	std::size_t child2Count =
		CountBVHLeafNodesAndCollectLeafSizes(tree->mChildNodes[2].get());

	std::size_t child3Count =
		CountBVHLeafNodesAndCollectLeafSizes(tree->mChildNodes[3].get());

	return child0Count + child1Count + child2Count + child3Count;
}





std::size_t RTIW::CountBVHLeafNodesAndCollectLeafSizes(const BVH8Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	if (tree->IsLeaf())
	{
		auto& leafSizes = Benchmarker::Get()->mCountingStats.back().mLeafSizes;
		leafSizes.Add(tree->mPrimitiveCount);
		return 1;
	}


	std::size_t leafNodeCounts[8]{};
	for (std::size_t i = 0; i < 8; i++)
	{
		leafNodeCounts[i] =
			CountBVHLeafNodesAndCollectLeafSizes(tree->mChildNodes[i].get());
	}

	return std::accumulate(leafNodeCounts, leafNodeCounts + 8, 0);
}





std::size_t RTIW::CountBVHLeafNodesAndCollectLeafSizes(
	const BVH2Node_VariableChild* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}

	if (tree->IsLeaf())
	{
		auto& leafSizes = Benchmarker::Get()->mCountingStats.back().mLeafSizes;
		leafSizes.Add(tree->mChildPrimitives.size());
		return 1;
	}

	std::size_t leftLeafCount  =
		CountBVHLeafNodesAndCollectLeafSizes(tree->mLeftNode.get());

	std::size_t rightLeafCount =
		CountBVHLeafNodesAndCollectLeafSizes(tree->mRightNode.get());

	return leftLeafCount + rightLeafCount;
}





bool RTIW::HitBVH2_BM(const BVH2Node* node,
                      const ray& ray,
                      interval interval,
                      hit_record& hitRecord,
	                  int currentDepth,
	                  BVHPerRayStats& stats)
{
	if (!node)
	{
		return false;
	}

	++stats.mNumberOfNodesVisited;
	stats.mTraversalDepth = std::max(stats.mTraversalDepth,
	                                 static_cast<int64_t>(currentDepth));

	if (node->IsLeaf())
	{
		++stats.mNumberOfPrimitiveTests;
		return node->Primitive->hit(ray, interval, hitRecord);
	}

	++stats.mNumberOfAABBTests;
	if (!HitAABB(node->mBoundingBox, ray, interval))
	{
		return false;
	}

	hit_record leftHitRecord;
	hit_record rightHitRecord;

	bool hitLeft = false;
	bool hitRight = false;

	if (node->mLeftNode)
	{
		hitLeft = HitBVH2_BM(node->mLeftNode.get(),
		                            ray,
		                            interval,
		                            leftHitRecord,
		                            currentDepth + 1,
		                            stats);
	}

	if (hitLeft)
	{
		interval.max = leftHitRecord.t;
	}

	if (node->mRightNode)
	{
		hitRight = HitBVH2_BM(node->mRightNode.get(),
		                      ray,
		                      interval,
		                      rightHitRecord,
		                      currentDepth + 1,
		                      stats);
	}

	if (hitLeft && hitRight)
	{
		hitRecord = (leftHitRecord.t < rightHitRecord.t) ? leftHitRecord
		                                                 : rightHitRecord;
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





struct BBHitInfo
{
	std::size_t mIndex;
	interval mBBHitInterval;
};



bool RTIW::HitBVH4_BM(const BVH4Node* node,
                      const ray& ray,
                      interval validTimeInterval,
                      hit_record& hitRecord,
                      int currentDepth,
                      BVHPerRayStats& stats)
{
	if (!node)
	{
		return false;
	}

	++stats.mNumberOfNodesVisited;
	stats.mTraversalDepth = std::max(stats.mTraversalDepth,
	                                 static_cast<int64_t>(currentDepth));

	if (node->IsLeaf())
	{
		hit_record tempRec;
		bool hitAnything = false;
		double closestSoFar = validTimeInterval.max;

		for (std::size_t i = 0; i < node->mPrimitiveCount; i++)
		{
			++stats.mNumberOfPrimitiveTests;

			if (node->mChildPrimitives[i]->hit(
					ray,
					interval(validTimeInterval.min, closestSoFar),
					tempRec))
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
		++stats.mNumberOfAABBTests;

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

		if(HitBVH4_BM(node->mChildNodes[currentChildIndex].get(),
		           ray,
		           interval(validTimeInterval.min, closestPrimHitTSoFar),
		           tempPrimHitRecord,
		           currentDepth + 1,
		           stats))
		{
			hit = true;
			closestPrimHitTSoFar = tempPrimHitRecord.t;
			hitRecord = tempPrimHitRecord;
		}
	}

	return hit;
}





bool RTIW::HitBVH8_BM(const BVH8Node* node,
                      const ray& ray,
                      interval validTimeInterval,
                      hit_record& hitRecord,
                      int currentDepth,
                      BVHPerRayStats& stats)
{
	if (!node)
	{
		return false;
	}

	++stats.mNumberOfNodesVisited;
	stats.mTraversalDepth = std::max(stats.mTraversalDepth,
	                                 static_cast<int64_t>(currentDepth));

	if (node->IsLeaf())
	{
		hit_record tempRec;
		bool hitAnything = false;
		double closestSoFar = validTimeInterval.max;

		for (std::size_t i = 0; i < node->mPrimitiveCount; i++)
		{
			++stats.mNumberOfPrimitiveTests;

			if (node->mChildPrimitives[i]->hit(
					ray,
					interval(validTimeInterval.min, closestSoFar),
					tempRec))
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
		++stats.mNumberOfAABBTests;

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

		if(HitBVH8_BM(node->mChildNodes[currentChildIndex].get(),
		              ray,
		              interval(validTimeInterval.min, closestPrimHitTSoFar),
		              tempPrimHitRecord,
		              currentDepth + 1,
		              stats))
		{
			hit = true;
			closestPrimHitTSoFar = tempPrimHitRecord.t;
			hitRecord = tempPrimHitRecord;
		}
	}

	return hit;
}





bool RTIW::HitBVH2_VariableChild_BM(const BVH2Node_VariableChild* node,
                                    const ray& ray,
                                    interval validTimeInterval,
                                    hit_record& hitRecord,
                                    int currentDepth,
                                    BVHPerRayStats& stats)
{
	if (!node)
	{
		return false;
	}

	++stats.mNumberOfNodesVisited;
	stats.mTraversalDepth = std::max(stats.mTraversalDepth,
	                                 static_cast<int64_t>(currentDepth));

	++stats.mNumberOfAABBTests;
	if (!HitAABB(node->mBoundingBox, ray, validTimeInterval))
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
			++stats.mNumberOfPrimitiveTests;

			if (primitive->hit(
					ray,
					interval(validTimeInterval.min, closestSoFar),
					tempRec))
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
		hitLeft = HitBVH2_VariableChild_BM(node->mLeftNode.get(),
		                                   ray,
		                                   validTimeInterval,
		                                   leftHitRecord,
		                                   currentDepth + 1,
		                                   stats);
	}

	if (hitLeft)
	{
		validTimeInterval.max = leftHitRecord.t;
	}

	if (node->mRightNode)
	{
		hitRight = HitBVH2_VariableChild_BM(node->mRightNode.get(),
		                                    ray,
		                                    validTimeInterval,
		                                    rightHitRecord,
		                                    currentDepth + 1,
		                                    stats);
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





