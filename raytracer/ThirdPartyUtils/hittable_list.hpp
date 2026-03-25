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





	class SceneObject
	{
	public:
		virtual bool hit(const ray&  r,
						 interval ray_t,
						 hit_record& rec) const = 0;

		virtual void GatherPrimitives(
				std::vector<const Primitive*>& out) const = 0;

	protected:
		std::vector<std::shared_ptr<Primitive>> mPrimitives;
	};




	// =======================================================================
	//		Scene : Pass this into the camera to render
	// -----------------------------------------------------------------------
	class Scene
	{
	public:
		Scene() {}

		void clear() { objects.clear(); }

		std::size_t add(std::shared_ptr<SceneObject> object);

		bool hit(const ray&  r,
				 interval ray_t,
				 hit_record& rec) const;

		std::shared_ptr<SceneObject> Get(std::size_t index) { return objects[index]; }

	private:
		std::vector<std::shared_ptr<SceneObject>> objects;
	};





}





#endif // HITTABLE_LIST_H





