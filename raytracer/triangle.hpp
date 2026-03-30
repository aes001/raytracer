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
#include "hittable.hpp"
#include "bvh.hpp"
#include "mat44.hpp"
#include "vec3.hpp"




// =======================================================================
//		Forward Declarations
// -----------------------------------------------------------------------
namespace RACCPPM
{
	class PPMImage;
}


namespace RTIW
{
	class Primitive;
	class ray;
}





namespace RTIW
{





	//========================================================================
	//		Triangle Simplified : A simplified triangle class
	//------------------------------------------------------------------------
	//		An immutable triangle class that also avoids virtual dispatch
	//		This class should no longer be used
	//------------------------------------------------------------------------
	class TriangleSimp
	{
	public:
		TriangleSimp(vec3 v0, vec3 v1, vec3 v2);

		bool hit(const ray& r, interval ray_t, hit_record& rec) const;

		vec3 Normal() const;

		AABB GetBoundingBox() const;

	public:
		const vec3 mV0;
		const vec3 mV1;
		const vec3 mV2;
	};





	class Triangle : public Primitive
	{
	public:
		Triangle(vec3 v0, vec3 v1, vec3 v2);

		bool hit(const ray& r,
				 interval ray_t,
				 hit_record& rec) const override;

		AABB GetBoundingBox() const override;

		vec3& v0();
		vec3& v1();
		vec3& v2();

		vec3 Normal() const;

		vec3& operator[](const size_t i);

		void Transform(const Mat44d& transform);


	private:
		vec3& GetVertex(const size_t i);


	private:
		vec3 mV0;
		vec3 mV1;
		vec3 mV2;
	};




}




#endif // TRIANGLE_H





