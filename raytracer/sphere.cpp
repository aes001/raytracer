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
bool sphere::hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const
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

		if (rootOne <= ray_tmax && rootOne >= ray_tmin)
		{
			acceptedRoot = rootOne;
			hit = true;
		}
		else if (rootTwo <= ray_tmax && rootTwo >= ray_tmin)
		{
			acceptedRoot = rootTwo;
			hit = true;
		}

		if (hit)
		{
			rec.t = acceptedRoot;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p - center) / radius;
		}
	}

	return hit;
}





