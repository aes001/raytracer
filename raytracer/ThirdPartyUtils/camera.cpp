/*	NAME:
 *		camera.cpp
 *
 *	DESCRIPTION:
 *		Implementation for a render camera.
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 21-SEP-25
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "camera.hpp"
#include "ppm.hpp"
#include "hittable.hpp"
#include "RaccPPMToRTIWBridge.inl"
#include "RaccDebug.hpp"





using namespace RTIW;





// ===========================================================================
//		camera : Render
// ---------------------------------------------------------------------------
bool camera::Render(const hittable& world, RACCPPM::PPMImage& imageBuffer)
{
	Initialize();

	bool imageBufferValid = imageBuffer.GetWidth() >= mImageWidth &&
							imageBuffer.GetHeight() >= mImageHeight;

	RACC_REQUIRE(imageBufferValid,
				 "Error: Image buffer dimension is narrower than camera " <<
				 "output!");

	bool isRenderingSuccessful = false;

	if(imageBufferValid)
	{
		// Render
		for (int j = 0; j < mImageHeight; j++)
		{
			std::clog << "\rScanlines remaining: " <<
						 (mImageHeight - j)        <<
						 ' '                       <<
						 std::flush;

			for (int i = 0; i < mImageWidth; i++)
			{
				vec3 pixelCenter = mPixelZeroLocation +
								   (i * mPixelDeltaX) +
								   (j * mPixelDeltaY);
				vec3 rayDirection = pixelCenter - mCameraCenter;
				ray r(mCameraCenter, rayDirection);

				color rayColor = RayColor(r, world);
				imageBuffer.PixelAt(i, j) =
					RaccPPMToRTIWBridge::ColorToRaccRBG(rayColor);
			}
		}

		isRenderingSuccessful = true;
		std::clog << "\rRendering Done!";
		std::clog << "\rDone.                          \n";
	}
	else
	{
		std::cerr << "Error: "
				  << "Image buffer dimension is narrower than camera output!";
	}


	return isRenderingSuccessful;
}





int camera::GetWidth() const
{
	return mImageWidth;
}




int camera::GetHeight() const
{
	return mImageHeight;
}





void camera::SetWidth(int width)
{
	mImageWidth = width;
}




void camera::SetAspectRatio(double aspectRatio)
{
	mAspectRatio = aspectRatio;
}




// ===========================================================================
//		camera : Initialize
// ---------------------------------------------------------------------------
void camera::Initialize()
{
	// Set up output image size in pixels
	mImageHeight = mImageWidth / mAspectRatio;
	mImageHeight = (mImageHeight < 1) ? 1 : mImageHeight;

	const double focalLength = 1.0;
	const double viewportHeight = 2.0;
	const double viewportWidth =
		viewportHeight * (double(mImageWidth)/double(mImageHeight));

	mCameraCenter = point3(0, 0, 0);


	// Calculate vector going the horizontal direction of the viewport
	const vec3 viewportXVector = vec3(viewportWidth, 0, 0);

	// Calculate vector going the vertical direction of the viewport
	const vec3 viewportYVector = vec3(0, -viewportHeight, 0);


	// Pixel Delta
	mPixelDeltaX = viewportXVector / mImageWidth;
	mPixelDeltaY = viewportYVector / mImageHeight;


	// Calculate the location of the top left corner of the viewport
	const vec3 viewportUpperLeftCorner = mCameraCenter -
										 vec3(0, 0, focalLength) -
										 (viewportXVector / 2) -
										 (viewportYVector / 2);


	// Calulate the location of the 0th pixel
	mPixelZeroLocation =
		viewportUpperLeftCorner + (0.5 * (mPixelDeltaX + mPixelDeltaY));
}





// ===========================================================================
//		camera : Ray Color
// ---------------------------------------------------------------------------
color camera::RayColor(const ray& r, const hittable& world) const
{
	color colorRet;

	hit_record rec;
	if (world.hit(r, interval(0, infinity), rec))
	{
		colorRet = 0.5 * (rec.normal + color(1, 1, 1));
	}
	else
	{
		vec3 unitDirection = unit_vector(r.direction());
		double a = 0.5 * (unitDirection.y() + 1.0);
		colorRet = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	return colorRet;
}





