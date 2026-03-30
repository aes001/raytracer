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
set (RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH "RayTracer-TD_MS_BVH2")
RACC_DEFINE_EXECUTABLE_TARGET(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH})


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

# RAY TRACER: NAIVE
target_sources(${PROJECT_NAME}
	PRIVATE
		${RT_BASIC_SRC}
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${RT_BASIC_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		${RT_BASIC_LIBS}
)



# RAY TRACER: BINARY TOP DOWN MEDIAN SPLIT BVH
target_compile_definitions(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH}
	PRIVATE
		BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH=1
)

target_sources(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH}
	PRIVATE
		${RT_BASIC_SRC}
)

target_include_directories(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH}
	PRIVATE
		${RT_BASIC_INCLUDE_DIRS}
)

target_link_libraries(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH}
	PUBLIC
		${RT_BASIC_LIBS}
)



