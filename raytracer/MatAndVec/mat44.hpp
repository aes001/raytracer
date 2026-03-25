/*	NAME:
 *		Mat44.hpp
 *
 *	DESCRIPTION:
 *		4 x 4 Matrix tools
 *
 *	CONTRIBUTORS:
 *		 Racc 22-MAR-26
*/





#ifndef MAT_44_D_HPP
#define MAT_44_D_HPP





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "matrices.hpp"
#include "vec.hpp"

// Standard Library
#include <cmath>
#include <cassert>
#include <cstdlib>






namespace RTIW
{





	using Mat44d = Mat<double, 4, 4>;





	constexpr Mat44d kIdentity44d = { {
		1., 0., 0., 0.,
		0., 1., 0., 0.,
		0., 0., 1., 0.,
		0., 0., 0., 1.
	} };






	inline
	vec4 operator*(Mat44d const& mat, vec4 const& vec) noexcept
	{
		float x = (mat.At(0, 0) * vec[0]) + (mat.At(0, 1) * vec[1]) + (mat.At(0, 2) * vec[2]) + (mat.At(0, 3) * vec[3]);
		float y = (mat.At(1, 0) * vec[0]) + (mat.At(1, 1) * vec[1]) + (mat.At(1, 2) * vec[2]) + (mat.At(1, 3) * vec[3]);
		float z = (mat.At(2, 0) * vec[0]) + (mat.At(2, 1) * vec[1]) + (mat.At(2, 2) * vec[2]) + (mat.At(2, 3) * vec[3]);
		float w = (mat.At(3, 0) * vec[0]) + (mat.At(3, 1) * vec[1]) + (mat.At(3, 2) * vec[2]) + (mat.At(3, 3) * vec[3]);

		return { x, y, z, w };
	}





	inline
	vec3 TransformPoint(const Mat44d& m, const vec3& p)
	{
		return vec3(
			m.At(0,0) * p.x() + m.At(0,1) * p.y() + m.At(0,2) * p.z() + m.At(0,3),
			m.At(1,0) * p.x() + m.At(1,1) * p.y() + m.At(1,2) * p.z() + m.At(1,3),
			m.At(2,0) * p.x() + m.At(2,1) * p.y() + m.At(2,2) * p.z() + m.At(2,3)
		);
	}





	// Functions:
	inline
	Mat44d transpose(Mat44d const& aM) noexcept
	{
		Mat44d ret;
		for( std::size_t i = 0; i < 4; ++i )
		{
			for( std::size_t j = 0; j < 4; ++j )
			{
				ret.At(j,i) = aM.At(i,j);
			}
		}
		return ret;
	}





	inline
	Mat44d MakeRotationX(float aAngle) noexcept
	{
		Mat44d R = kIdentity44d;

		R.At(1, 1) = cos(aAngle);
		R.At(1, 2) = -sin(aAngle);
		R.At(2, 1) = sin(aAngle);
		R.At(2, 2) = cos(aAngle);

		return R;
	}





	inline
	Mat44d MakeRotationY(float aAngle) noexcept
	{
		Mat44d R = kIdentity44d;

		R.At(0, 0) = (float)cos(aAngle);
		R.At(0, 2) = (float)sin(aAngle);
		R.At(2, 0) = -(float)sin(aAngle);
		R.At(2, 2) = (float)cos(aAngle);

		return R;
	}





	inline
	Mat44d MakeRotationZ(float aAngle) noexcept
	{
		Mat44d R = kIdentity44d;

		R.At(0, 0) = (float)cos(aAngle);
		R.At(0, 1) = -(float)sin(aAngle);
		R.At(1, 0) = (float)sin(aAngle);
		R.At(1, 1) = (float)cos(aAngle);

		return R;
	}





	inline
	Mat44d MakeTranslation(vec3 aTranslation) noexcept
	{
		Mat44d R = kIdentity44d;

		R.At(0, 3) = aTranslation[0];
		R.At(1, 3) = aTranslation[1];
		R.At(2, 3) = aTranslation[2];

		return R;
	}





	inline
	Mat44d MakeScaling(float aSX, float aSY, float aSZ) noexcept
	{
		Mat44d R = kIdentity44d;

		R.At(0, 0) = aSX;
		R.At(1, 1) = aSY;
		R.At(2, 2) = aSZ;

		return R;
	}





}





#endif // MAT_44_D_HPP





