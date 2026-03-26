/*	NAME:
 *		model.cpp
 *
 *	DESCRIPTION:
 *		model.hpp implementation
 *
 *	CONTRIBUTORS:
 *		 Racc 19-FEB-26
*/





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "triangulatedMesh.hpp"
#include "rapidobj/rapidobj.hpp"
#include "fmt/base.h"
#include "fmt/format.h"
#include "convenientNumerics.hpp"
#include "mat44.hpp"
#include <memory>





using namespace RTIW;
using namespace rapidobj;






TriangulatedMesh::TriangulatedMesh(const std::string& modelPath)
{
	Result obj = ParseFile(modelPath);

	if (obj.error)
	{
		std::string errorMessage =
			fmt::format("Error loading obj file '{}'", modelPath);
		std::runtime_error e(errorMessage);
		throw e;
	}

	Triangulate(obj);

	for (const auto& shape : obj.shapes)
	{
		vec3 temp[3];
		size_t count = 0;
		for (const auto& index: shape.mesh.indices)
		{
			 temp[count % 3] = {
				obj.attributes.positions[index.position_index * 3 + 0],
				obj.attributes.positions[index.position_index * 3 + 1],
				obj.attributes.positions[index.position_index * 3 + 2]};

			count++;

			if ((count % 3) == 0)
			{
				mPrimitives.push_back(std::make_shared<Triangle>(temp[0], temp[1], temp[2]));
			}
		}
	}
}





bool TriangulatedMesh::hit(const ray& r, interval ray_t, hit_record& rec) const
{
	hit_record tempHR;
	rec.t = RACC::kDoubleMax;
	bool hit = false;

	for (const auto& triangle: mPrimitives)
	{
		if (triangle->hit(r, ray_t, tempHR))
		{
			hit = true;
			ray_t.max = tempHR.t;
			rec = tempHR;
		}
	}

	return hit;
}




void TriangulatedMesh::GatherPrimitives(std::vector<const Primitive*>& out) const
{
	for (const auto& prim : mPrimitives)
	{
		out.push_back(prim.get());
	}
}





void TriangulatedMesh::Transform(const Mat44d& transform)
{
	for (const std::shared_ptr<Primitive>& primitive : mPrimitives)
	{
		auto triangle = std::static_pointer_cast<Triangle>(primitive);
		triangle->Transform(transform);
	}
}

//BoundingBox Model::GetBoundingBox() const
//{
//	vec3 min{ RACC::kDoubleMin, RACC::kDoubleMin, RACC::kDoubleMin };
//	vec3 max{ RACC::kDoubleMax, RACC::kDoubleMax, RACC::kDoubleMax };
//
//	for (const auto& triangle : mTriangles)
//	{
//		const auto& v0 = triangle.mV0;
//		const auto& v1 = triangle.mV1;
//		const auto& v2 = triangle.mV2;
//
//		min[0] = std::min({v0.x(), v1.x(), v2.x(), min.x()});
//		min[1] = std::min({v0.y(), v1.y(), v2.y(), min.y()});
//		min[2] = std::min({v0.z(), v1.z(), v2.z(), min.z()});
//
//		max[0] = std::max({v0.x(), v1.x(), v2.x(), max.x()});
//		max[1] = std::max({v0.y(), v1.y(), v2.y(), max.y()});
//		max[2] = std::max({v0.z(), v1.z(), v2.z(), max.z()});
//	}
//
//	return BoundingBox{min, max};
//}





