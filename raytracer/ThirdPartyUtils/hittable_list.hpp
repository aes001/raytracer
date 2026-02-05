/*	NAME:
 *		hittable_list.hpp
 *
 *	DESCRIPTION:
 *		Header for a list object of hittable objects
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 02-SEP-25
*/





#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
// RTIW headers
#include "hittable.hpp"

// Standard Library
#include <memory>
#include <vector>





namespace RTIW
{





	// =======================================================================
	//		hittable list
	// -----------------------------------------------------------------------
	class hittable_list : public hittable
	{
	public:
		hittable_list() {}
		hittable_list(std::shared_ptr<hittable> object);

		void clear() { objects.clear(); }

		void add(std::shared_ptr<hittable> object);

		bool hit(const ray&  r,
				 interval ray_t,
				 hit_record& rec) const override;

		std::vector<std::shared_ptr<hittable>> objects;
	};





}





#endif // HITTABLE_LIST_H





