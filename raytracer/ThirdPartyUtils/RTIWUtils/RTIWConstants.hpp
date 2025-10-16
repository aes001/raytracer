/*	NAME:
 *		rtweekend.hpp
 *
 *	DESCRIPTION:
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
*/





#ifndef RTIW_CONSTANTS_HPP
#define RTIW_CONSTANTS_HPP

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


// C++ Std Usings

// using std::make_shared;
// using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#endif // RTIW_CONSTANTS_HPP
