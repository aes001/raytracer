/*	NAME:
 *		triangle.cpp
 *
 *	DESCRIPTION:
 *		Implementation of triangle primitive
 *		Implements Moeller - Trumbore ray/triangle intersection algorithm
 *
 *	CONTRIBUTORS:
 *		 Racc 05-FEB-26
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "triangle.hpp"

// Standard Library
#include <algorithm>





using namespace RTIW;





Triangle::Triangle(vec3 v0, vec3 v1, vec3 v2)
	: mV0(v0)
	, mV1(v1)
	, mV2(v2)
{
}





bool Triangle::hit(const ray& r, interval ray_t, hit_record& rec) const
{
	const vec3 e1 = mV1 - mV0;
	const vec3 e2 = mV2 - mV0;

	const vec3 p = cross(r.direction(), e2);

	const double determinant = dot(e1, p);


	// Use the determinant to check if the triangle is facing away from the
	// camera
	const bool backfacing = determinant < 0.0;

	if (backfacing)
	{
		return false;
	}


	// Calculate u
	const vec3 tvec = r.origin() - mV0;

	const double u = dot(tvec, p);
	const bool uInvalid = u < 0.0 || u > determinant;

	if (uInvalid)
	{
		return false;
	}


	// Calculate v
	const vec3 q = cross(tvec, e1);

	const double v = dot(r.direction(), q);
	const bool vInvalid = v < 0.0 || u + v > determinant;

	if (vInvalid)
	{
		return false;
	}


	// Calculate t
	const double invDeterminant = 1.0 / determinant;

	double t = dot(e2, q) * invDeterminant;
	const bool tInvalid = !(ray_t.surrounds(t));

	if (tInvalid)
	{
		return false;
	}



	// Confirmed hit
	rec.t = t;
	rec.p = r.at(t);
	vec3 faceNormal = cross(e1, e2);
	rec.set_face_normal(r, faceNormal / faceNormal.length());

	return true;
}





vec3& Triangle::v0()
{
	return mV0;
}





vec3& Triangle::v1()
{
	return mV1;
}





vec3& Triangle::v2()
{
	return mV2;
}





vec3 Triangle::Normal() const
{
	return cross(mV1 - mV0, mV2 - mV0);
}





vec3& Triangle::operator[](const size_t i)
{
	return GetVertex(i);
}





vec3& Triangle::GetVertex(const size_t i)
{
	switch(i)
	{
		case 0:
			return mV0;
		case 1:
			return mV1;
		case 2:
			return mV2;
		default:
			throw std::out_of_range("Invalid triangle vertex");
	}
}





BoundingBox Triangle::GetBoundingBox() const
{
	vec3 min = {std::min({mV0.x(), mV1.x(), mV2.x()}),
				std::min({mV0.y(), mV1.y(), mV2.y()}),
				std::min({mV0.z(), mV1.z(), mV2.z()})};

	vec3 max = {std::max({mV0.x(), mV1.x(), mV2.x()}),
				std::max({mV0.y(), mV1.y(), mV2.y()}),
				std::max({mV0.z(), mV1.z(), mV2.z()})};

	return BoundingBox{min, max};
}





void Triangle::Transform(const Mat44d& transform)
{
	mV0 = TransformPoint(transform, mV0);
	mV1 = TransformPoint(transform, mV1);
	mV2 = TransformPoint(transform, mV2);
}





TriangleSimp::TriangleSimp(vec3 v0, vec3 v1, vec3 v2)
	: mV0(v0)
	, mV1(v1)
	, mV2(v2)
{
}





bool TriangleSimp::hit(const ray& r, interval ray_t, hit_record& rec) const
{
	const vec3 e1 = mV1 - mV0;
	const vec3 e2 = mV2 - mV0;

	const vec3 p = cross(r.direction(), e2);

	const double determinant = dot(e1, p);


	// Use the determinant to check if the triangle is facing away from the
	// camera
	const bool backfacing = determinant < 0.0;

	if (backfacing)
	{
		return false;
	}


	// Calculate u
	const vec3 tvec = r.origin() - mV0;

	const double u = dot(tvec, p);
	const bool uInvalid = u < 0.0 || u > determinant;

	if (uInvalid)
	{
		return false;
	}


	// Calculate v
	const vec3 q = cross(tvec, e1);

	const double v = dot(r.direction(), q);
	const bool vInvalid = v < 0.0 || u + v > determinant;

	if (vInvalid)
	{
		return false;
	}


	// Calculate t
	const double invDeterminant = 1.0 / determinant;

	double t = dot(e2, q) * invDeterminant;
	const bool tInvalid = !(ray_t.surrounds(t));

	if (tInvalid)
	{
		return false;
	}



	// Confirmed hit
	rec.t = t;
	rec.p = r.at(t);
	vec3 faceNormal = cross(e1, e2);
	rec.set_face_normal(r, faceNormal / faceNormal.length());

	return true;
}





vec3 TriangleSimp::Normal() const
{
	return cross(mV1 - mV0, mV2 - mV0);
}





BoundingBox TriangleSimp::GetBoundingBox() const
{
	vec3 min = {std::min({mV0.x(), mV1.x(), mV2.x()}),
				std::min({mV0.y(), mV1.y(), mV2.y()}),
				std::min({mV0.z(), mV1.z(), mV2.z()})};

	vec3 max = {std::max({mV0.x(), mV1.x(), mV2.x()}),
				std::max({mV0.y(), mV1.y(), mV2.y()}),
				std::max({mV0.z(), mV1.z(), mV2.z()})};

	return BoundingBox{min, max};
}






