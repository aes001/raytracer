#include "ppm.hpp"
#include "RTIWConglomerate.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"





int main()
{
    // World
	RTIW::hittable_list world;

	world.add(std::make_shared<RTIW::sphere>(RTIW::point3(0, 0, -1), 0.5));
	world.add(std::make_shared<RTIW::sphere>(RTIW::point3(0,-100.5,-1), 100));

	const double aspectRatio = 16.0 / 9.0;
	const int imageWidth = 1280;
	const int sampleCount = 100;

	RTIW::camera cam;
	cam.SetAspectRatio(aspectRatio);
	cam.SetWidth(imageWidth);
	cam.SetSampleCount(sampleCount);

	auto image = RACCPPM::PPMMaker::NewPPMImage("OutputImage",
												imageWidth,
												aspectRatio);

	cam.Render(world, image);

	image.SaveAs("OutputImage");

	return 0;
}





