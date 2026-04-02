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





#define USE_BVH \
	BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH || \
	BVH4_TOP_DOWN_EVEN_SPLIT || \
	BVH8_TOP_DOWN_EVEN_SPLIT





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
bool camera::Render(const Scene& world, RACCPPM::PPMImage& imageBuffer)
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
		// Build BVH
#if USE_BVH
		std::vector<const Primitive*> allPrimitives;
		world.GatherAllWorldPrimitives(allPrimitives);
#endif // USE_BVH

#if BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH
		std::unique_ptr<BVHBinaryNode> bvhTree = BuildBVH2_MedianSplit(allPrimitives, 0, allPrimitives.size());
#elif BVH4_TOP_DOWN_EVEN_SPLIT
		std::unique_ptr<BVH4Node> bvhTree = BuildBVH4_EvenSplit(allPrimitives, 0, allPrimitives.size());
#elif BVH8_TOP_DOWN_EVEN_SPLIT
		std::unique_ptr<BVH8Node> bvhTree = BuildBVH8_EvenSplit(allPrimitives, 0, allPrimitives.size());
#endif // BVH BUILD STRATEGY


		// Render
		for (int j = 0; j < mImageHeight; j++)
		{
			std::clog << "\rScanlines remaining: " <<
						 (mImageHeight - j)        <<
						 ' '                       <<
						 std::flush;

			for (int i = 0; i < mImageWidth; i++)
			{
				color pixelColor;
				for (int sample = 0; sample < mSamplesPerPixel; sample++)
				{
					ray r = GetRay(i, j);
#if BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH
					pixelColor += RayColor_BVH2(bvhTree.get(), r, world);
#elif BVH4_TOP_DOWN_EVEN_SPLIT
					pixelColor += RayColor_BVH4(bvhTree.get(), r, world);
#elif BVH8_TOP_DOWN_EVEN_SPLIT
					pixelColor += RayColor_BVH8(bvhTree.get(), r, world);
#else // NAIVE
					pixelColor += RayColor(r, world);
#endif // BVH HIT ALGORITHM
				}
				static const interval intensity(0.000, 0.999);

				// Average out all the sampled values
				pixelColor = pixelColor * mPixelSampleScale;

				// Make sure our result is within 0 to 1
				pixelColor[0] = intensity.clamp(pixelColor[0]);
				pixelColor[1] = intensity.clamp(pixelColor[1]);
				pixelColor[2] = intensity.clamp(pixelColor[2]);

				auto rgbVal = RaccPPMToRTIWBridge::ColorToRaccRBG(pixelColor);

				// std::cout << "R " << rgbVal.Red() << "\n";
				// std::cout << "G " << rgbVal.Green() << "\n";
				// std::cout << "B " << rgbVal.Blue() << "\n";

				imageBuffer.PixelAt(i, j) = rgbVal;
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





int camera::GetSampleCount() const
{
	return mSamplesPerPixel;
}





void camera::SetWidth(int width)
{
	mImageWidth = width;
}




void camera::SetAspectRatio(double aspectRatio)
{
	mAspectRatio = aspectRatio;
}




void camera::SetSampleCount(int sampleCount)
{
	mSamplesPerPixel = sampleCount;
}




// ===========================================================================
//		camera : Initialize
// ---------------------------------------------------------------------------
void camera::Initialize()
{
	// Set up output image size in pixels
	mImageHeight = mImageWidth / mAspectRatio;
	mImageHeight = (mImageHeight < 1) ? 1 : mImageHeight;

	mPixelSampleScale = 1.0 / mSamplesPerPixel;

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
color camera::RayColor(const ray& r, const Scene& world) const
{
	color colorRet;

	hit_record rec;
	if (world.hit(r, interval(0, infinity), rec))
	{
#if !RENDER_SURFACE_NORMAL
		vec3 direction = random_on_hemisphere(rec.normal);
		colorRet = 0.5 * RayColor(ray(rec.p, direction), world);
#else
		colorRet = 0.5 * (rec.normal + color(1, 1, 1));
#endif // RENDER_SURFACE_NORMAL
	}
	else
	{
		vec3 unitDirection = unit_vector(r.direction());
		double a = 0.5 * (unitDirection.y() + 1.0);
		colorRet = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	return colorRet;
}





color camera::RayColor_BVH2(BVHBinaryNode* bvh,
                                 const ray& r,
                                 const Scene& world) const
{
	color colorRet;

	hit_record rec;
	if (HitBVH2(bvh, r, interval(0, infinity), rec))
	{
#if !RENDER_SURFACE_NORMAL
		vec3 direction = random_on_hemisphere(rec.normal);
		colorRet = 0.5 * RayColor_BVH2(bvh, ray(rec.p, direction), world);
#else
		colorRet = 0.5 * (rec.normal + color(1, 1, 1));
#endif // RENDER_SURFACE_NORMAL
	}
	else
	{
		vec3 unitDirection = unit_vector(r.direction());
		double a = 0.5 * (unitDirection.y() + 1.0);
		colorRet = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	return colorRet;

}





color camera::RayColor_BVH4(BVH4Node* bvh,
                            const ray& r,
                            const Scene& world) const
{
	color colorRet;

	hit_record rec;
	if (HitBVH4(bvh, r, interval(0, infinity), rec))
	{
#if !RENDER_SURFACE_NORMAL
		vec3 direction = random_on_hemisphere(rec.normal);
		colorRet = 0.5 * RayColor_BVH4(bvh, ray(rec.p, direction), world);
#else
		colorRet = 0.5 * (rec.normal + color(1, 1, 1));
#endif // RENDER_SURFACE_NORMAL
	}
	else
	{
		vec3 unitDirection = unit_vector(r.direction());
		double a = 0.5 * (unitDirection.y() + 1.0);
		colorRet = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	return colorRet;
}





color camera::RayColor_BVH8(BVH8Node* bvh,
                            const ray& r,
                            const Scene& world) const
{
	color colorRet;

	hit_record rec;
	if (HitBVH8(bvh, r, interval(0, infinity), rec))
	{
#if !RENDER_SURFACE_NORMAL
		vec3 direction = random_on_hemisphere(rec.normal);
		colorRet = 0.5 * RayColor_BVH8(bvh, ray(rec.p, direction), world);
#else
		colorRet = 0.5 * (rec.normal + color(1, 1, 1));
#endif // RENDER_SURFACE_NORMAL
	}
	else
	{
		vec3 unitDirection = unit_vector(r.direction());
		double a = 0.5 * (unitDirection.y() + 1.0);
		colorRet = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	return colorRet;
}





ray camera::GetRay(int i, int j) const
{
	// Construct a camera ray originating from the origin and directed at
	// randomly sampled point around the pixel location i, j.

	auto offset = SampleSquare();
	auto pixelSample = mPixelZeroLocation +
	                   ((i + offset.x()) * mPixelDeltaX) +
	                   ((j + offset.y()) * mPixelDeltaY);

	auto rayOrigin = mCameraCenter;
	auto rayDirection = pixelSample - rayOrigin;

	return ray(rayOrigin, rayDirection);
}





vec3 camera::SampleSquare() const
{
	// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
	return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}





