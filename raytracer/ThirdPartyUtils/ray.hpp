/*	NAME:
 *		ray.hpp
 *
 *	DESCRIPTION:
 *		Header for ray.cpp
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 18-AUG-25
*/





#ifndef RAY_H
#define RAY_H




// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "vec3.hpp"





namespace RTIW
{





	// =======================================================================
	//		ray
	// -----------------------------------------------------------------------
	class ray
	{
	public:
											ray() {}
											ray(const point3& origin,
												const vec3& direction);

		const point3&						origin() const  { return orig; }
		const vec3&							direction() const { return dir; }

		point3								at(double t) const { return orig + t*dir; }


	private:
		point3 orig;
		vec3 dir;
	};





}





#endif // RAY_H





