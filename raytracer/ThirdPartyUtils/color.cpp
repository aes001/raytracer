
/*	NAME:
 *		color.cpp
 *
 *	DESCRIPTION:
 *		Implementation for color related utils
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 12-OCT-25
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "color.hpp"
#include <iostream>





namespace RTIW
{






	// =======================================================================
	//		write_color : writes a color to the given output stream
	// -----------------------------------------------------------------------
	void write_color(std::ostream& out, const color& pixel_color)
	{
		auto r = pixel_color.x();
		auto g = pixel_color.y();
		auto b = pixel_color.z();

		// Translate the [0,1] component values to the byte range [0,255].
		int rbyte = int(255.999 * r);
		int gbyte = int(255.999 * g);
		int bbyte = int(255.999 * b);

		// Write out the pixel color components.
		out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
	}




}





