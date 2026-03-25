/*	NAME:
 *		vec.hpp
 *
 *	DESCRIPTION:
 *		Vector class conglomeration.
 *
 *	CONTRIBUTORS:
 *		 Racc 24-MAR-26
*/





#ifndef VEC_HPP
#define VEC_HPP





// ============================================================================
//		Includes
// ----------------------------------------------------------------------------
#include "vec3.hpp"
#include "vec4.hpp"





namespace RTIW
{





	inline
	vec4 ToPoint(const vec3& v) noexcept
	{
		return vec4{v.x(), v.y(), v.z(), 1.0};
	}





	inline
	vec4 ToDirection(const vec3& v) noexcept
	{
		return vec4{v.x(), v.y(), v.z(), 0.0};
	}





	inline
	vec3 ToVec3(const vec4& v) noexcept
	{
		return vec3(v.x(), v.y(), v.z());
	}





}





#endif // VEC_HPP





