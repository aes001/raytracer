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
//Scene::Scene(std::shared_ptr<SceneObject> object)
//{
//	add(object);
//}





// ===========================================================================
//		hittable_list : add
// ---------------------------------------------------------------------------
std::size_t Scene::add(std::shared_ptr<SceneObject> object)
{
	objects.push_back(object);
	return objects.size() - 1;
}





// ===========================================================================
//		hittable_list : hit
// ---------------------------------------------------------------------------
bool Scene::hit(const ray& r, interval ray_t, hit_record& rec) const
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





//BoundingBox Scene::GetBoundingBox() const
//{
//	return BoundingBox();
//}





