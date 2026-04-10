#include "camera.hpp"
#include "fmt/base.h"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "instrumenter.hpp"
#include "mat44.hpp"
#include "ppm.hpp"
#include "RTIWConglomerate.hpp"
#include "sphere.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "triangulatedMesh.hpp"
#include "triangle.hpp"
#include <memory>





int main(int argc, char *argv[])
{
    // World
	RTIW::Scene world;

	if (argc > 2)
	{
		std::cerr << "Invalid argument count";
		return -1;
	}


	if (argc == 2)
	{
		BuildMetaData::sSceneName = std::string(argv[1]);
		auto object = std::make_shared<RTIW::TriangulatedMesh>(argv[1]);
		world.add(object);
	}
	else
	{
		std::size_t monkeyIdx = world.add(std::make_shared<RTIW::TriangulatedMesh>("../../data/suzanne.obj"));
		auto monkeyPtr = std::static_pointer_cast<RTIW::TriangulatedMesh>(world.Get(monkeyIdx));

		RTIW::Mat44d monkeyTransform;
		monkeyTransform += RTIW::MakeTranslation( { 0, 0, -3. } );
		monkeyTransform += RTIW::MakeRotationY( 45.0_deg);
		monkeyPtr->Transform(monkeyTransform);
	}

#if !BENCHMARK_BUILD
	const double aspectRatio = 16.0 / 9.0;
	const int imageWidth = 640;
	const int sampleCount = 10;

	RTIW::camera cam;
	cam.SetAspectRatio(aspectRatio);
	cam.SetWidth(imageWidth);
	cam.SetSampleCount(sampleCount);

	auto image = RACCPPM::PPMMaker::NewPPMImage("OutputImage",
												imageWidth,
												aspectRatio);

	cam.Render(world, image);

	stbi_write_png("OutputPNG.png", imageWidth, imageWidth / aspectRatio, 3, image.Pixels().data(), 3 * imageWidth);

	return 0;
#else
	RenderSettingsInfo renderSettings;
	renderSettings.mAspectRatio = 16.0 / 9.0;
	renderSettings.mImageWidth = 640;
	renderSettings.mSampleCount = 1;
	renderSettings.mPrimitiveCount = world.CountPrimitives();

	RTIW::camera cam;
	cam.SetAspectRatio(renderSettings.mAspectRatio);
	cam.SetWidth(renderSettings.mImageWidth);
	cam.SetSampleCount(renderSettings.mSampleCount);

	auto image = RACCPPM::PPMMaker::NewPPMImage("OutputImage",
	                                            renderSettings.mImageWidth,
	                                            renderSettings.mAspectRatio);

	auto* bmPtr = Benchmarker::Get();



	// Warm Up the renderer
	const std::size_t warmUpCount = 5;
	bmPtr->SetState(BenchmarkerState::kWarmingUp);
	for (std::size_t i = 0; i < warmUpCount; ++i)
	{
		std::clog << "\rWarming up... " <<
					 i + 1 <<
					 " / " <<
					 warmUpCount <<
					 std::flush;

		cam.Render(world, image);
	}

	std::clog << "\n";


	// Do timing Stats
	bmPtr->SetState(BenchmarkerState::kRenderTimingStats);
	const std::size_t renderTimingTrialsCount = 20;
	for (std::size_t i = 0; i < renderTimingTrialsCount; ++i)
	{
		std::clog << "\rBenchmarking render timings, " <<
		             cam.GetSampleCount() << " samples... " <<
		             i + 1 <<
		             " / " <<
		             renderTimingTrialsCount<<
		             std::flush;

		bmPtr->mOverallTimingStats.push_back(BM_OverallTimingStats());
		bmPtr->mRenderSettings.push_back(renderSettings);
		cam.Render(world, image);
	}


	std::clog << "\n";


	renderSettings.mSampleCount = 10;
	cam.SetSampleCount(renderSettings.mSampleCount);
	for (std::size_t i = 0; i < renderTimingTrialsCount; ++i)
	{
		std::clog << "\rBenchmarking render timings, " <<
		             cam.GetSampleCount() << " samples... " <<
		             i + 1 <<
		             " / " <<
		             renderTimingTrialsCount<<
		             std::flush;

		bmPtr->mOverallTimingStats.push_back(BM_OverallTimingStats());
		bmPtr->mRenderSettings.push_back(renderSettings);
		cam.Render(world, image);
	}


	std::clog << "\n";


	renderSettings.mSampleCount = 100;
	cam.SetSampleCount(renderSettings.mSampleCount);
	for (std::size_t i = 0; i < renderTimingTrialsCount; ++i)
	{
		std::clog << "\rBenchmarking render timings, " <<
		             cam.GetSampleCount() << " samples... " <<
		             i + 1 <<
		             " / " <<
		             renderTimingTrialsCount<<
		             std::flush;

		bmPtr->mOverallTimingStats.push_back(BM_OverallTimingStats());
		bmPtr->mRenderSettings.push_back(renderSettings);
		cam.Render(world, image);
	}



	std::clog << "\n";



	// Do counting Stats
	renderSettings.mSampleCount = 10;
	cam.SetSampleCount(renderSettings.mSampleCount);

	bmPtr->SetState(BenchmarkerState::kRenderCountingStats);
	std::clog << "\nCounting rendering things...\n";
	bmPtr->mCountingStats.push_back(BM_CountingStats());
	bmPtr->mRenderSettings.push_back(renderSettings);
	cam.Render(world, image);

	const auto& countingStats = bmPtr->mCountingStats.back();

	fmt::print("Tree depth                 = {}\n", countingStats.mTreeDepth);
	fmt::print("Total number of nodes      = {}\n", countingStats.mTotalNumberOfNodes);
	fmt::print("Total number of leaf nodes = {}\n", countingStats.mTotalNumberOfLeafNodes);
	fmt::print("Total number of rays shot  = {}\n", countingStats.mNumberOfRaysShot);
	fmt::print("Memory usage               = {}\n", countingStats.mMemoryUsage);

	fmt::print("\n");
	fmt::print("Average traversal depth           = {}\n", countingStats.mTraversalDepth.Average());
	fmt::print("Average Number of nodes visited   = {}\n", countingStats.mNumberOfNodesVisited.Average());
	fmt::print("Average Number of AABB test       = {}\n", countingStats.mNumberOfAABBTest.Average());
	fmt::print("Average Number of primitive tests = {}\n", countingStats.mNumberOfPrimitiveTests.Average());



	// Write the data
	Benchmarker::Get()->WriteData();

	fmt::print("Done!\n");

	return 0;
#endif
}





