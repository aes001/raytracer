/*	NAME:
 *		sphere.hpp
 *
 *	DESCRIPTION:
 *		Header for sphere object implementation from rayracing.github.io
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 26-AUG-25
*/






#ifndef SPHERE_HPP
#define SPHERE_HPP





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "hittable.hpp"
#include "vec3.hpp"





namespace RTIW
{





	// =======================================================================
	//		Sphere
	// -----------------------------------------------------------------------
	class sphere : public hittable
	{
	public:
											sphere(const point3& center,
												   double radius);


		bool								hit(const ray& r,
												double ray_tmin,
												double ray_tmax,
												hit_record& rec) const override;


	private:
		point3 center;
		double radius;

	};




}





#endif // SPHERE_HPP





