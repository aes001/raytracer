/* NAME:
 *		instrumenter.cpp
 *
 * DESCRIPTION:
 *		Utility to benchmark the ray tracer
 *
 * CONTRIBUTORS:
 *		Racc 05-APR-26
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "instrumenter.hpp"

// Standard Library
#include <fstream>




Benchmarker* Benchmarker::Get()
{
	static Benchmarker benchmarker;
	return &benchmarker;
}





BenchmarkerState Benchmarker::GetState()
{
	return mState;
}





void Benchmarker::SetState(BenchmarkerState state)
{
	mState = state;
}





BenchmarkerState Benchmarker::AdvanceState()
{
	std::size_t state = static_cast<std::size_t>(mState);
	state = (state + 1) % static_cast<std::size_t>(kInstrumenerStateCount);
	mState = static_cast<BenchmarkerState>(state);

	return mState;
}





void Benchmarker::WriteData() const
{
	bool isSaved = false;

	std::string fileNameBase = BuildMetaData::sSceneName + "_"
	                         + BuildMetaData::kMetaDataAbrvd.TreeType + "_"
	                         + BuildMetaData::kMetaDataAbrvd.Approach + "_"
	                         + BuildMetaData::kMetaDataAbrvd.SplitStrategy;

	auto& renderSettingsData = Benchmarker::Get()->mRenderSettings;

	// Write the render timings
	std::ofstream timingsFile;
	timingsFile.open("Timings-" + fileNameBase + ".csv", std::ios::out | std::ios::binary);

	if (timingsFile.is_open())
	{

		timingsFile << "TreeType" << ","
		            << "Approach" << ","
		            << "SplitStrategy" << ","
		            << "BuildTime" << ","
		            << "BuildTimeUnit" << ","
		            << "RenderTime" << ","
		            << "RenderTimeUnit" << ","
		            << "SampleCount" << "\n";

		auto& timingsData = Benchmarker::Get()->mOverallTimingStats;
		for (std::size_t i = 0; i < timingsData.size(); ++i)
		{
			timingsFile << BuildMetaData::kMetaData.TreeType << ","
			            << BuildMetaData::kMetaData.Approach << ","
			            << BuildMetaData::kMetaData.SplitStrategy << ","
			            << timingsData[i].mBVHBuildTimeResults.count() << ","
			            << "microseconds" << ","
			            << timingsData[i].mRenderTime.count() << ","
			            << "miliseconds" << ","
			            << renderSettingsData[i].mSampleCount << "\n";
		}
	}



	// Write the countings
	std::ofstream countingsFile;
	countingsFile.open("Countings-" + fileNameBase + ".csv", std::ios::out | std::ios::binary);
	auto& countingsData = Benchmarker::Get()->mCountingStats.back();

	if (countingsFile.is_open())
	{
		countingsFile << "TreeType" << ","
		              << "Approach" << ","
		              << "SplitStrategy" << ","
		              << "TreeDepth" << ","
		              << "TotalNumberOfNodes" << ","
		              << "TotalNumberOfLeafNodes" << ","
		              << "NumberOfRaysShot" << ","
		              << "MemoryUsage" << ","
		              << "AverageLeafSize" << ","
		              << "AverageTraversalDepth" << ","
		              << "AverageNumberOfNodesVisited" << ","
		              << "AverageNumberOfAABBTests" << ","
		              << "AverageNumberOfPrimitiveTests" << ","
		              << "SumOfLeafSize" << ","
		              << "SumOfTraversalDepth" << ","
		              << "SumOfNumberOfNodesVisited" << ","
		              << "SumOfNumberOfAABBTest" << ","
		              << "SumOfNumberOfPrimitiveTests" << ","
		              << "MinLeafSize" << ","
		              << "MinTraversalDepth" << ","
		              << "MinNumberOfNodesVisited" << ","
		              << "MinNumberOfAABBTest" << ","
		              << "MinNumberOfPrimitiveTests" << ","
		              << "MaxLeafSize" << ","
		              << "MaxTraversalDepth" << ","
		              << "MaxNumberOfNodesVisited" << ","
		              << "MaxNumberOfAABBTest" << ","
		              << "MaxNumberOfPrimitiveTests" << ","
		              << "CountOfLeafSize" << ","
		              << "CountOfTraversalDepth" << ","
		              << "CountOfNumberOfNodesVisited" << ","
		              << "CountOfNumberOfAABBTest" << ","
		              << "CountOfNumberOfPrimitiveTests" << "\n";


		countingsFile << BuildMetaData::kMetaData.TreeType << ","
		              << BuildMetaData::kMetaData.Approach << ","
		              << BuildMetaData::kMetaData.SplitStrategy << ","
		              << countingsData.mTreeDepth << ","
		              << countingsData.mTotalNumberOfNodes << ","
		              << countingsData.mTotalNumberOfLeafNodes << ","
		              << countingsData.mNumberOfRaysShot << ","
		              << countingsData.mMemoryUsage << ","
		              << countingsData.mLeafSizes.Average() << ","
		              << countingsData.mTraversalDepth.Average() << ","
		              << countingsData.mNumberOfNodesVisited.Average() << ","
		              << countingsData.mNumberOfAABBTest.Average() << ","
		              << countingsData.mNumberOfPrimitiveTests.Average() << ","
		              << countingsData.mLeafSizes.mSum << ","
		              << countingsData.mTraversalDepth.mSum << ","
		              << countingsData.mNumberOfNodesVisited.mSum << ","
		              << countingsData.mNumberOfAABBTest.mSum << ","
		              << countingsData.mNumberOfPrimitiveTests.mSum << ","
		              << countingsData.mLeafSizes.mMin << ","
		              << countingsData.mTraversalDepth.mMin << ","
		              << countingsData.mNumberOfNodesVisited.mMin << ","
		              << countingsData.mNumberOfAABBTest.mMin << ","
		              << countingsData.mNumberOfPrimitiveTests.mMin << ","
		              << countingsData.mLeafSizes.mMax << ","
		              << countingsData.mTraversalDepth.mMax << ","
		              << countingsData.mNumberOfNodesVisited.mMax << ","
		              << countingsData.mNumberOfAABBTest.mMax << ","
		              << countingsData.mNumberOfPrimitiveTests.mMax << ","
		              << countingsData.mLeafSizes.mCount << ","
		              << countingsData.mTraversalDepth.mCount << ","
		              << countingsData.mNumberOfNodesVisited.mCount << ","
		              << countingsData.mNumberOfAABBTest.mCount << ","
		              << countingsData.mNumberOfPrimitiveTests.mCount << "\n";
	}



	// Write the histogram file
	std::ofstream histogramFile;
	histogramFile.open("Histogram-" + fileNameBase + ".csv", std::ios::out | std::ios::binary);

	if (histogramFile.is_open())
	{
		histogramFile << "TreeType" << ","
		              << "Approach" << ","
		              << "SplitStrategy" << ","
		              << "FieldName" << ","
		              << "Bucket" << ","
		              << "Count" << "\n";

		auto writer = [&histogramFile] (const auto& data, const std::string& fieldName)
			{
				for (std::int64_t i = data.kmHistogramStart;
				     i <= data.kmHistogramEnd;
				     ++i)
				{
					histogramFile << BuildMetaData::kMetaData.TreeType << ","
					              << BuildMetaData::kMetaData.Approach << ","
					              << BuildMetaData::kMetaData.SplitStrategy << ","
					              << fieldName << ","
					              << i << ","
					              << data.mHistogram[i - data.kmHistogramStart] << "\n";
				}
			};

		auto& leafHistogramData = countingsData.mLeafSizes;
		writer(leafHistogramData, "LeafSizes");

		auto& traversalDepthData = countingsData.mTraversalDepth;
		writer(traversalDepthData, "TraversalDepth");

		auto& numberOfNodesVisitedData = countingsData.mNumberOfNodesVisited;
		writer(numberOfNodesVisitedData, "NumberOfNodesVisited");

		auto& numberOfAABBTestData = countingsData.mNumberOfAABBTest;
		writer(numberOfAABBTestData, "NumberOfAABBTests");

		auto& numberOfPrimitiveTestsData = countingsData.mNumberOfPrimitiveTests;
		writer(numberOfPrimitiveTestsData, "NumberOfPrimitiveTests");
	}
}





