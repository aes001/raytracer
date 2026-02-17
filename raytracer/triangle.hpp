/*	NAME:
 *		triangle.hpp
 *
 *	DESCRIPTION:
 *		Triangle primitive
 *
 *	CONTRIBUTORS:
 *		 Racc 05-FEB-26
*/





#ifndef TRIANGLE_H
#define TRIANGLE_H





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "vec3.hpp"
#include "hittable.hpp"





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





	class Triangle : public hittable
	{
	public:
		Triangle(vec3 v0, vec3 v1, vec3 v2);

		bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

		vec3& v0();
		vec3& v1();
		vec3& v2();

		vec3 Normal() const;

		vec3& operator[](const size_t i);
		vec3& operator[](const int i);

	private:
		vec3& GetVertex(const size_t i);


	private:
		vec3 mV0;
		vec3 mV1;
		vec3 mV2;
	};




}




#endif // TRIANGLE_H





