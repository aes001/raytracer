/*	NAME:
 *		interval.hpp
 *
 *	DESCRIPTION:
 *		Implementation of an interval class. This file is to be included only
 *		in rtweekend.hpp. I don't know how I feel about that.
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 17-SEP-25
*/





#ifndef INTERVAL_H
#define INTERVAL_H




#include "RTIWUtils/RTIWConstants.hpp"





namespace RTIW
{





	class interval
	{
	  public:
		double min, max;

		interval() : min(+infinity), max(-infinity) {} // Default interval is empty

		interval(double min, double max) : min(min), max(max) {}

		double size() const { return max - min; }

		bool contains(double x) const { return min <= x && x <= max; }

		bool surrounds(double x) const { return min < x && x < max; }

		static const interval empty, universe;
	};





	inline const interval interval::empty    = interval(+infinity, -infinity);
	inline const interval interval::universe = interval(-infinity, +infinity);





}





#endif // INTERVAL_H





