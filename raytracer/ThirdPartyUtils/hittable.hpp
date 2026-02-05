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
#include "vec3.hpp"
#include "interval.hpp"




namespace RTIW
{
	class interval;
}




namespace RTIW
{





	// =======================================================================
	//		hit_record : Stores information about a ray hit
	// -----------------------------------------------------------------------
	class hit_record
	{
	public:
		point3								p;
		vec3								normal;
		double								t;
		bool								front_face;


		void set_face_normal(const ray& r, const vec3& outward_normal)
		{
			// Sets the hit record normal vector.
			// NOTE: the parameter `outward_normal` is assumed to have unit length.

			front_face = dot(r.direction(), outward_normal) < 0;
			normal = front_face ? outward_normal : -outward_normal;
		}
	};





	// =======================================================================
	//		hittable : Abstract class for a hittable object
	// -----------------------------------------------------------------------
	class hittable
	{
	public:
		virtual ~hittable() = default;

		virtual bool hit(const ray& r,
						 interval ray_t,
						 hit_record& rec) const = 0;
	};





}





#endif // HITTABLE_H





