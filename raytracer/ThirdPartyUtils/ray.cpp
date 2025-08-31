/*	NAME:
 *		ray.cpp
 *
 *	DESCRIPTION:
 *		Implementation for a ray class
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 18-AUG-25
*/





#include "ray.hpp"





using namespace RTIW;





// ===========================================================================
//		ray : Constructor
// ---------------------------------------------------------------------------
ray::ray(const point3& origin, const vec3& direction)
	: orig(origin)
	, dir(direction)
{}





