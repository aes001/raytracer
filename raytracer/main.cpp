#include "mat44.hpp"
#include "ppm.hpp"
#include "RTIWConglomerate.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "triangle.hpp"
#include "triangulatedMesh.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include <memory>





#define BINARY_BVH 1





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
}





