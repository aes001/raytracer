############################################################
#
# ██████   █████  ██    ██ ████████ ██████   █████   ██████ ███████ ██████
# ██   ██ ██   ██  ██  ██     ██    ██   ██ ██   ██ ██      ██      ██   ██
# ██████  ███████   ████      ██    ██████  ███████ ██      █████   ██████
# ██   ██ ██   ██    ██       ██    ██   ██ ██   ██ ██      ██      ██   ██
# ██   ██ ██   ██    ██       ██    ██   ██ ██   ██  ██████ ███████ ██   ██
#___________________________________________________________
#===========================================================
#	Contents
#-----------------------------------------------------------
#	1. Setup
#	2. Sources
#	3. Libraries
#	4. Combine
#	5. Link
#-----------------------------------------------------------





#===========================================================
#	1. Setup
#-----------------------------------------------------------
project(RayTracer)

# Naive
RACC_DEFINE_EXECUTABLE_TARGET(${PROJECT_NAME})

# With Binary BVH
set (RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH "RayTracer-TD_ES_BVH2")
RACC_DEFINE_EXECUTABLE_TARGET(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH})

# With BVH4
set (RT_BVH4_TOP_DOWN_MEDIAN_SPLIT "RayTracer-TD_ES_BVH4")
RACC_DEFINE_EXECUTABLE_TARGET(${RT_BVH4_TOP_DOWN_MEDIAN_SPLIT})

# With BVH8
set (RT_BVH8_TOP_DOWN_EVEN_SPLIT "RayTracer-TD_ES_BVH8")
RACC_DEFINE_EXECUTABLE_TARGET(${RT_BVH8_TOP_DOWN_EVEN_SPLIT})

# With BVH2 NAIVE SAH
set (RT_BVH2_TOP_DOWN_NAIVE_SAH "RayTracer-TD_NSAH_BVH2")
RACC_DEFINE_EXECUTABLE_TARGET(${RT_BVH2_TOP_DOWN_NAIVE_SAH})


# Also make our libraries available
add_subdirectory(RaccPPM)
add_subdirectory(RaccDebug)




#===========================================================
#	2. Sources
#-----------------------------------------------------------
set (THIRD_PARTY_UTILS_SRC
	"ThirdPartyUtils/RTIWUtils/RTIWConstants.hpp"
	"ThirdPartyUtils/bvh.hpp"
	"ThirdPartyUtils/bvh.cpp"
	"ThirdPartyUtils/camera.cpp"
	"ThirdPartyUtils/camera.hpp"
	"ThirdPartyUtils/color.hpp"
	"ThirdPartyUtils/hittable.hpp"
	"ThirdPartyUtils/hittable_list.cpp"
	"ThirdPartyUtils/hittable_list.hpp"
	"ThirdPartyUtils/interval.hpp"
	"ThirdPartyUtils/RaccPPMToRTIWBridge.inl"
	"ThirdPartyUtils/ray.cpp"
	"ThirdPartyUtils/ray.hpp"
	"ThirdPartyUtils/RTIWConglomerate.hpp"
	"ThirdPartyUtils/sphere.cpp"
	"ThirdPartyUtils/sphere.hpp"
	"ThirdPartyUtils/vec3.hpp"
	"ThirdPartyUtils/vec4.hpp"
	"ThirdPartyUtils/vec.hpp"
)

set (MY_OWN_STUFF_SRC
	"convenientNumerics.hpp"
	"MatAndVec/matrices.hpp"
	"MatAndVec/mat44.hpp"
	"triangulatedMesh.cpp"
	"triangulatedMesh.hpp"
	"triangle.cpp"
	"triangle.hpp"
)


set (MY_OWN_STUFF_HEADERS
	"convenientNumerics.hpp"
	"MatAndVec/matrices.hpp"
	"MatAndVec/mat44.hpp"
	"triangulatedMesh.hpp"
	"triangle.hpp"
)





#===========================================================
#	3. Libraries
#-----------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt
	GIT_TAG        407c905e45ad75fc29bf0f9bb7c5c2fd3475976f)
FetchContent_MakeAvailable(fmt)

set (LIBS_UHSP
	"${RACC_LIBS_DIR}/rapidobj/include"
)





#===========================================================
#	4. Combine
#-----------------------------------------------------------
set (RT_BASIC_SRC
	main.cpp
	main.hpp
	${THIRD_PARTY_UTILS_SRC}
	${MY_OWN_STUFF_SRC}
)


set (RT_BASIC_INCLUDE_DIRS
	${MY_OWN_STUFF_HEADERS}
	MatAndVec
	RaccDebug
	RaccPPM
	ThirdPartyUtils
	${LIBS_UHSP}
)


set (RT_BASIC_LIBS
	RaccPPM
	rapidobj
	fmt::fmt
	stb-image
)






#===========================================================
#	5. Link
#-----------------------------------------------------------
function(TARGET_LINK_FILES projectName)
	target_sources(${projectName}
		PRIVATE
			${RT_BASIC_SRC}
	)

	target_include_directories(${projectName}
		PRIVATE
			${RT_BASIC_INCLUDE_DIRS}
	)

	target_link_libraries(${projectName}
		PUBLIC
			${RT_BASIC_LIBS}
	)
endfunction()


# RAY TRACER: NAIVE
TARGET_LINK_FILES(${PROJECT_NAME})



# RAY TRACER: BINARY TOP DOWN MEDIAN SPLIT BVH
TARGET_LINK_FILES(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH})
target_compile_definitions(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH}
	PRIVATE
		BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH=1
)



# RAY TRACER: BVH4 TOP DOWN EVEN SPLIT
TARGET_LINK_FILES(${RT_BVH4_TOP_DOWN_MEDIAN_SPLIT})
target_compile_definitions(${RT_BVH4_TOP_DOWN_MEDIAN_SPLIT}
	PRIVATE
		BVH4_TOP_DOWN_EVEN_SPLIT=1
)



# RAY TRACER: BVH8 TOP DOWN EVEN SPLIT
TARGET_LINK_FILES(${RT_BVH8_TOP_DOWN_EVEN_SPLIT})
target_compile_definitions(${RT_BVH8_TOP_DOWN_EVEN_SPLIT}
	PRIVATE
		BVH8_TOP_DOWN_EVEN_SPLIT=1
)



# RAY TRACER: BVH2 TOP DOWN NAIVE SAH
TARGET_LINK_FILES(${RT_BVH2_TOP_DOWN_NAIVE_SAH})
target_compile_definitions(${RT_BVH2_TOP_DOWN_NAIVE_SAH}
	PRIVATE
		BVH2_TOP_DOWN_NAIVE_SAH=1
)



