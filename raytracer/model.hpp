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
#include "triangle.hpp"
#include "vec3.hpp"
#include "hittable.hpp"





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




	class Model : public hittable
	{
	public:
		Model(const std::string& modelPath);

		virtual bool hit(const ray& r,
						 interval ray_t,
						 hit_record& rec) const override;

		BoundingBox GetBoundingBox() const override;

	private:
		std::vector<TriangleSimp> mTriangles;
	};





}






#endif // MODEL_HPP
