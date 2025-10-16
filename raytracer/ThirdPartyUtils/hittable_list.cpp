/*	NAME:
 *		hittable_list.cpp
 *
 *	DESCRIPTION:
 *		Implementation for a list object of hittable objects
 *		Taken from Ray Tracing in One Weekend.
 *
 *	CONTRIBUTORS:
 *		 Ray Tracing in One Weekend Authors (Peter Shirley, Trevor David Black,
 *		 Steve Hollasch)
 *		 Racc 02-SEP-25
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "hittable_list.hpp"
#include "interval.hpp"





using namespace RTIW;





// ===========================================================================
//		hittable_list : constructor with object
// ---------------------------------------------------------------------------
hittable_list::hittable_list(std::shared_ptr<hittable> object)
{
	add(object);
}





// ===========================================================================
//		hittable_list : add
// ---------------------------------------------------------------------------
void hittable_list::add(std::shared_ptr<hittable> object)
{
	objects.push_back(object);
}





// ===========================================================================
//		hittable_list : hit
// ---------------------------------------------------------------------------
bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = ray_t.max;

	for (const auto& object : objects)
	{
		if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}





