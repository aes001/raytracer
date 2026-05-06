/* NAME:
 *		instrumenter.hpp
 *
 * DESCRIPTION:
 *		Header for instrumenter.cpp
 *		Utility to benchmark the ray tracer
 *
 * CONTRIBUTORS:
 *		Racc 05-APR-26
*/





#ifndef INSTRUMENTER_HPP
#define INSTRUMENTER_HPP





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "RaccDebug/RaccDebug.hpp"

// Standard Library
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <vector>






template <int64_t HistogramStart, int64_t HistogramEnd>
struct DiscreteDistributionStats
{
	static_assert(HistogramEnd >= HistogramStart,
	              "Error HistogramEnd must be >= HistogramStart");

	std::uint64_t mCount = 0;
	std::int64_t mSum = 0;
	std::int64_t mSumSquares = 0;

	std::uint64_t mOverflowCount = 0;
	std::uint64_t mUnderflowCount = 0;

	std::int64_t mMin = std::numeric_limits<std::int64_t>::max();
	std::int64_t mMax = std::numeric_limits<std::int64_t>::min();

	std::array<std::uint64_t, HistogramEnd - HistogramStart + 1> mHistogram{};

	const std::int64_t kmHistogramStart = HistogramStart;
	const std::int64_t kmHistogramEnd = HistogramEnd;

	void Add(std::int64_t value);

	double Average() const;

};




template <int64_t HistogramStart, int64_t HistogramEnd>
void DiscreteDistributionStats<HistogramStart, HistogramEnd>::Add(std::int64_t value)
{
	++mCount;
	mSum += value;
	mSumSquares += value * value;
	mMin = std::min(mMin, value);
	mMax = std::max(mMax, value);

	int indexToAdd = value - HistogramStart;

	if (value > HistogramEnd)
	{
		++mOverflowCount;
		return;
	}

	if (value < HistogramStart)
	{
		++mUnderflowCount;
		return;
	}

	std::size_t histogramBucket =
		static_cast<std::size_t>(value - HistogramStart);

	++mHistogram[histogramBucket];
}





template <int64_t HistogramStart, int64_t HistogramEnd>
double DiscreteDistributionStats<HistogramStart, HistogramEnd>::Average() const
{
	return mCount ?
		static_cast<double>(mSum) / static_cast<double>(mCount) : 0.0;
}





enum BenchmarkerState : std::size_t
{
	kWarmingUp = 0,
	kRenderTimingStats,
	kRenderCountingStats,
	kHotPathStats,

	kInstrumenerStateCount
};





using BMT_BuildTime = std::chrono::microseconds;
using BMT_RenderTime = std::chrono::milliseconds;
using BMT_TraversalTime = std::chrono::nanoseconds;





struct BM_OverallTimingStats
{
	BMT_BuildTime mBVHBuildTimeResults;
	BMT_RenderTime mRenderTime;
};


struct BM_CountingStats
{
	int mTreeDepth = 0;
	int mTotalNumberOfNodes = 0;
	int mTotalNumberOfLeafNodes = 0;

	std::uint64_t mNumberOfRaysShot = 0;

	DiscreteDistributionStats<0, 64> mLeafSizes;

	std::size_t mMemoryUsage;

	DiscreteDistributionStats<-1, 64>  mTraversalDepth; // This is every time we traverse the tree so per ray.
	DiscreteDistributionStats<0, 255>  mNumberOfNodesVisited; // This is also per ray
	DiscreteDistributionStats<0, 255>  mNumberOfAABBTest; // This is also per ray until it hits a primitive
	DiscreteDistributionStats<0, 255>  mNumberOfPrimitiveTests;
};


struct BM_HotPathStats
{
	// How long it takes to reach a primitive or conclude that it doesn't hit a primitive
	BMT_TraversalTime mAverageTraversalTime;
};





namespace BuildMetaData
{





	enum class BVHBuildApproach
	{
		kNone,
		kTopDown,
		kBottomUp
	};





	enum class BVHSplitStrategy
	{
		kNone,
		kEvenSplit,
		kSAH
	};





	enum class BVHTreeType
	{
		kNone,
		kBVH2,
		kBVH4,
		kBVH8,
		kBVH2VariableChild
	};





	inline constexpr const char* GetApproachStr(BVHBuildApproach approach)
	{
		switch(approach)
		{
			case BVHBuildApproach::kNone:
				return "None";
			case BVHBuildApproach::kTopDown:
				return "Top Down";
			case BVHBuildApproach::kBottomUp:
				return "Bottom Up";
		}
	}





	inline constexpr const char* GetSplitStrategyStr(BVHSplitStrategy strat)
	{
		switch(strat)
		{
			case BVHSplitStrategy::kNone:
				return "None";
			case BVHSplitStrategy::kEvenSplit:
				return "Even Split";
			case BVHSplitStrategy::kSAH:
				return "SAH";
		}
	}





	inline constexpr const char* GetBVHTreeType(BVHTreeType type)
	{
		switch(type)
		{
			case BVHTreeType::kNone:
				return "None";
			case BVHTreeType::kBVH2:
				return "BVH2";
			case BVHTreeType::kBVH4:
				return "BVH4";
			case BVHTreeType::kBVH8:
				return "BVH8";
			case BVHTreeType::kBVH2VariableChild:
				return "BVH2 - Variable Leaf Node Size";
		}
	}





	inline constexpr const char* GetApproachStrAbrvd(BVHBuildApproach approach)
	{
		switch(approach)
		{
			case BVHBuildApproach::kNone:
				return "None";
			case BVHBuildApproach::kTopDown:
				return "TD";
			case BVHBuildApproach::kBottomUp:
				return "BU";
		}
	}





	inline constexpr const char* GetSplitStrategyStrAbrvd(BVHSplitStrategy strat)
	{
		switch(strat)
		{
			case BVHSplitStrategy::kNone:
				return "None";
			case BVHSplitStrategy::kEvenSplit:
				return "ES";
			case BVHSplitStrategy::kSAH:
				return "SAH";
		}
	}





	inline constexpr const char* GetBVHTreeTypeAbrvd(BVHTreeType type)
	{
		switch(type)
		{
			case BVHTreeType::kNone:
				return "None";
			case BVHTreeType::kBVH2:
				return "BVH2";
			case BVHTreeType::kBVH4:
				return "BVH4";
			case BVHTreeType::kBVH8:
				return "BVH8";
			case BVHTreeType::kBVH2VariableChild:
				return "BVH2_VLS";
		}
	}





#if BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH
	constexpr BVHBuildApproach kApproach      = BVHBuildApproach::kTopDown;
	constexpr BVHSplitStrategy kSplitStrategy = BVHSplitStrategy::kEvenSplit;
	constexpr BVHTreeType kTreeType           = BVHTreeType::kBVH2;
#elif BVH4_TOP_DOWN_EVEN_SPLIT
	constexpr BVHBuildApproach kApproach      = BVHBuildApproach::kTopDown;
	constexpr BVHSplitStrategy kSplitStrategy = BVHSplitStrategy::kEvenSplit;
	constexpr BVHTreeType kTreeType           = BVHTreeType::kBVH4;
#elif BVH8_TOP_DOWN_EVEN_SPLIT
	constexpr BVHBuildApproach kApproach      = BVHBuildApproach::kTopDown;
	constexpr BVHSplitStrategy kSplitStrategy = BVHSplitStrategy::kEvenSplit;
	constexpr BVHTreeType kTreeType           = BVHTreeType::kBVH8;
#elif BVH2_TOP_DOWN_NAIVE_SAH
	constexpr BVHBuildApproach kApproach      = BVHBuildApproach::kTopDown;
	constexpr BVHSplitStrategy kSplitStrategy = BVHSplitStrategy::kSAH;
	constexpr BVHTreeType kTreeType           = BVHTreeType::kBVH2VariableChild;
#elif BVH2_BOTTOM_UP_SAH
	constexpr BVHBuildApproach kApproach      = BVHBuildApproach::kBottomUp;
	constexpr BVHSplitStrategy kSplitStrategy = BVHSplitStrategy::kSAH;
	constexpr BVHTreeType kTreeType           = BVHTreeType::kBVH2;
#else
	constexpr BVHBuildApproach kApproach      = BVHBuildApproach::kNone;
	constexpr BVHSplitStrategy kSplitStrategy = BVHSplitStrategy::kNone;
	constexpr BVHTreeType kTreeType           = BVHTreeType::kNone;
#endif // BVH BUILD STRATEGY



	// Why did I make this... Eh it works...
	struct RunTimeBuildInfo
	{
		static RunTimeBuildInfo* Get() { static RunTimeBuildInfo bi; return &bi;};

		std::string mSceneName = "Suzanne";
		std::int64_t mPrimitiveCount = 0;
	};




	struct MetaData
	{
		const char* Approach;
		const char* SplitStrategy;
		const char* TreeType;
	};

	inline constexpr MetaData kMetaData {
		GetApproachStr(kApproach),
		GetSplitStrategyStr(kSplitStrategy),
		GetBVHTreeType(kTreeType)
	};


	inline constexpr MetaData kMetaDataAbrvd {
		GetApproachStrAbrvd(kApproach),
		GetSplitStrategyStrAbrvd(kSplitStrategy),
		GetBVHTreeTypeAbrvd(kTreeType)
	};





};





struct RenderSettingsInfo
{
	double mAspectRatio;
	int mImageWidth;
	int mSampleCount;
	std::size_t mPrimitiveCount;
};





class Benchmarker
{
public:
	static Benchmarker* Get();

	BenchmarkerState GetState();
	BenchmarkerState AdvanceState();
	void SetState(BenchmarkerState state);

	void WriteData() const;

public:
	BenchmarkerState mState = kWarmingUp;

	std::vector<RenderSettingsInfo> mRenderSettings;
	std::vector<BM_OverallTimingStats> mOverallTimingStats;
	std::vector<BM_CountingStats> mCountingStats;
	std::vector<BM_HotPathStats> mHotPathStats;

};





#endif // INSTRUMENTER_HPP





