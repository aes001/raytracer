#include <iostream>
#include "ppm.hpp"
#include "color.hpp"
#include "ray.hpp"





double HitSphere(const RTIW::point3& center, double radius, const RTIW::ray& r)
{
	RTIW::vec3 oc = center - r.origin();
	double a = r.direction().length_squared();
	double h = dot(r.direction(), oc);
	double c = oc.length_squared() - (radius * radius);

	double discriminant = h * h - a * c;

	double tValue;

	if(discriminant > 0)
	{
		tValue = (h - std::sqrt(discriminant)) / a;
	}
	else
	{
		tValue = -1.0;
	}

	return tValue;
}





RTIW::color RayColor(const RTIW::ray& r)
{
	RTIW::color colorRet;

	double hitSphereT = HitSphere(RTIW::point3(0, 0, -1), 0.5, r);

	if( hitSphereT > 0.0)
	{
		RTIW::vec3 normalVector = unit_vector(r.at(hitSphereT) - RTIW::vec3(0, 0, -1));
		colorRet = 0.5 * RTIW::color(normalVector.x() + 1.0,
							   normalVector.y() + 1.0,
							   normalVector.z() + 1.0);
	}
	else
	{
		RTIW::vec3 unitDirection = unit_vector(r.direction());
		double a = 0.5 * (unitDirection.y() + 1.0);
		colorRet = RTIW::color((1.0 - a) * RTIW::color(1.0, 1.0, 1.0) + a * RTIW::color(0.5, 0.7, 1.0));
	}

	return colorRet;
}




inline RACCPPM::RGBValue ColorToRaccRBG(const RTIW::color& color)
{
	return RACCPPM::RGBValue{
		static_cast<uint8_t>(color.x() * 255.999),
		static_cast<uint8_t>(color.y() * 255.999),
		static_cast<uint8_t>(color.z() * 255.999)};
}




int main()
{
	// Set up output image size in pixels
	const double aspectRatio = 16.0 / 9.0;
	const int imageWidth = 256;
	int imageHeight = imageWidth / aspectRatio;
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	RACCPPM::PPMImage ppmImage = RACCPPM::PPMMaker::NewPPMImage("OutputImage.ppm", imageWidth, imageHeight);


	// Camera
	const double focalLength = 1.0;
	const double viewportHeight = 2.0;
	const double viewportWidth = viewportHeight * (double(imageWidth)/double(imageHeight));
	const RTIW::point3 cameraCenter = RTIW::point3(0, 0, 0);


	// Calculate vector going the horizontal direction of the viewport
	const RTIW::vec3 viewportXVector = RTIW::vec3(viewportWidth, 0, 0);

	// Calculate vector going the vertical direction of the viewport
	const RTIW::vec3 viewportYVector = RTIW::vec3(0, -viewportHeight, 0);


	// Pixel Delta
	const RTIW::vec3 pixelDeltaX = viewportXVector / imageWidth;
	const RTIW::vec3 pixelDeltaY = viewportYVector / imageHeight;


	// Calculate the location of the top left corner of the viewport
	const RTIW::vec3 viewportUpperLeftCorner = cameraCenter - RTIW::vec3(0, 0, focalLength) - (viewportXVector / 2) - (viewportYVector / 2);


	// Calulate the location of the 0th pixel
	const RTIW::vec3 pixelZeroLocation = viewportUpperLeftCorner + (0.5 * (pixelDeltaX + pixelDeltaY));


	// Render
	for (int j = 0; j < imageHeight; j++)
	{
		std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
		for (int i = 0; i < imageWidth; i++)
		{
			RTIW::vec3 pixelCenter = pixelZeroLocation + (i * pixelDeltaX) + (j * pixelDeltaY);
			RTIW::vec3 rayDirection = pixelCenter - cameraCenter;
			RTIW::ray r(cameraCenter, rayDirection);

			RTIW::color rayColor = RayColor(r);
			ppmImage.PixelAt(i, j) = ColorToRaccRBG(rayColor);
		}
	}

	std::clog << "\rRendering Done! Saving file...";
	ppmImage.SaveAs("bastardo");

	std::clog << "\rDone.                          \n";


	return 0;
}
