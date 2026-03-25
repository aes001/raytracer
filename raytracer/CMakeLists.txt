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
#	4. Link
#-----------------------------------------------------------





#===========================================================
#	1. Setup
#-----------------------------------------------------------
project(RayTracer)
RACC_DEFINE_EXECUTABLE_TARGET(${PROJECT_NAME})
add_subdirectory(RaccPPM)
add_subdirectory(RaccDebug)




#===========================================================
#	2. Sources
#-----------------------------------------------------------
set (THIRD_PARTY_UTILS_SRC
	"ThirdPartyUtils/RTIWUtils/RTIWConstants.hpp"
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
#	4. Link
#-----------------------------------------------------------
target_sources(${PROJECT_NAME}
	PRIVATE
		main.cpp
		main.hpp
		${THIRD_PARTY_UTILS_SRC}
		${MY_OWN_STUFF_SRC}
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		MatAndVec
		RaccDebug
		RaccPPM
		ThirdPartyUtils
		${LIBS_UHSP}
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		RaccPPM
		rapidobj
		fmt::fmt
		stb-image
)






