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



	private:
		std::vector<std::shared_ptr<hittable>> mObjects;
	};





}






#endif // MODEL_HPP
