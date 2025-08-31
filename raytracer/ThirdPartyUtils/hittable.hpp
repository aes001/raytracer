/*	NAME:
 *		hittable.hpp
 *
 *	DESCRIPTION:
 *		Header for hittable objects abstraction
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 26-AUG-25
*/






#ifndef HITTABLE_H
#define HITTABLE_H





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "ray.hpp"





namespace RTIW
{





	// =======================================================================
	//		hit_record : Stores information about a ray hit
	// -----------------------------------------------------------------------
	class hit_record
	{
	public:
		point3 p;
		vec3 normal;
		double t;
	};





	// =======================================================================
	//		hittable : Abstract class for a hittable object
	// -----------------------------------------------------------------------
	class hittable
	{
	public:
		virtual							~hittable() = default;

		virtual bool					hit(const ray& r,
											double ray_tmin,
											double ray_tmax,
											hit_record& rec) const = 0;
	};





}





#endif // HITTABLE_H





