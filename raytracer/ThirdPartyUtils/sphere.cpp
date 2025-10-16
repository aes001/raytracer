/*	NAME:
 *		sphere.cpp
 *
 *	DESCRIPTION:
 *		Sphere object implementation from rayracing.github.io
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 26-AUG-25
*/





// =============================================================================
//		Includes
// -----------------------------------------------------------------------------
#include "sphere.hpp"





using namespace RTIW;





// =============================================================================
//		sphere : Construtor
// -----------------------------------------------------------------------------
sphere::sphere(const point3& center, double radius)
	: center(center)
	, radius(std::fmax(0,radius))
{}





// =============================================================================
//		sphere : hit
// -----------------------------------------------------------------------------
bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const
{
	vec3 oc = center - r.origin();
	double a = r.direction().length_squared();
	double h = dot(r.direction(), oc);
	double c = oc.length_squared() - (radius * radius);

	double discriminant = h * h - a * c;

	bool hit = false;
	if (discriminant > 0)
	{
		double sqrtd = std::sqrt(discriminant);

		double rootOne = (h - sqrtd) / a;
		double rootTwo = (h + sqrtd) / a;
		double acceptedRoot;

		if (ray_t.surrounds(rootOne))
		{
			acceptedRoot = rootOne;
			hit = true;
		}
		else if (ray_t.surrounds(rootTwo))
		{
			acceptedRoot = rootTwo;
			hit = true;
		}

		if (hit)
		{
			rec.t = acceptedRoot;
			rec.p = r.at(rec.t);
			vec3 outwardNormal = (rec.p - center) / radius;
			rec.set_face_normal(r, outwardNormal);
		}
	}

	return hit;
}





