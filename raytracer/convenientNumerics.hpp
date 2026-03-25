/*	NAME:
 *		convenientNumerics.hpp
 *
 *	DESCRIPTION:
 *		Convenient numerical stuff
 *
 *	CONTRIBUTORS:
 *		 Racc 19-FEB-26
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
// Standard Library
#include <limits>
#include <type_traits>
#include <numbers>
#include "RTIWUtils/RTIWConstants.hpp"




namespace RACC
{





	constexpr double kDoubleMax = std::numeric_limits<double>::max();
	constexpr double kDoubleMin = std::numeric_limits<double>::lowest();




//	template <typename T>
//	bool NearlyEqual(T a, T b)
//	{
//		static_assert(std::is_floating_point_v<T>,
//					  "Invalid type substitution for NearlyEqual() function");
//	}





}





