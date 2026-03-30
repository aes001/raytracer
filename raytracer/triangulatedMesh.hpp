/*	NAME:
 *		model.hpp
 *
 *	DESCRIPTION:
 *		A loaded 3D model
 *
 *	CONTRIBUTORS:
 *		 Racc 11-FEB-26
*/





#ifndef MODEL_HPP
#define MODEL_HPP





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "mat44.hpp"
#include "triangle.hpp"





// =======================================================================
//		Forward Declarations
// -----------------------------------------------------------------------
namespace RACCPPM
{
	class PPMImage;
}


namespace RTIW
{
	class ray;
}





namespace RTIW
{





	class TriangulatedMesh : public SceneObject
	{
	public:
		TriangulatedMesh(const std::string& modelPath);

		bool hit(const ray& r,
		         interval ray_t,
		         hit_record& rec) const override;


		void GatherPrimitives(
			std::vector<const Primitive*>& out) const override;

		void Transform(const Mat44d& transform);

	};





}






#endif // MODEL_HPP





