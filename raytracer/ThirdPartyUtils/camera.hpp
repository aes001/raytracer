/*	NAME:
 *		camera.hpp
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





#ifndef CAMERA_H
#define CAMERA_H





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "vec3.hpp"
#include "color.hpp"





// =======================================================================
//		Forward Declarations
// -----------------------------------------------------------------------
namespace RACCPPM
{
	class PPMImage;
}


namespace RTIW
{
	class hittable;
	class ray;
}





namespace RTIW
{





	class camera
	{
	public:
		bool								Render(const hittable& world, RACCPPM::PPMImage& imageBuffer);


		int									GetWidth() const;
		int									GetHeight() const;

		void								SetWidth(int width);
		void								SetAspectRatio(double aspectRatio);



	private:
		void								Initialize();

		color								RayColor(const ray& r,
													  const hittable& world) const;


		double								mAspectRatio = 1;
		int									mImageWidth = 100;

		// These should not be user setttable
		int									mImageHeight;
		point3								mCameraCenter;
		vec3								mPixelZeroLocation;
		vec3								mPixelDeltaX;
		vec3								mPixelDeltaY;
	};





}





#endif // CAMERA_H





