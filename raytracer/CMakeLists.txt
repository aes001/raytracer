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
)

set (MY_OWN_STUFF_SRC
	"model.cpp"
	"model.hpp"
	"triangle.cpp"
	"triangle.hpp"
)






set (LIBS_UHSP
	"${RACC_LIBS_DIR}/glfw-3.4/include"
)






target_sources(${PROJECT_NAME}
	PRIVATE
		main.cpp
		main.hpp
		${THIRD_PARTY_UTILS_SRC}
		${MY_OWN_STUFF_SRC}
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		RaccPPM
		ThirdPartyUtils
		RaccDebug
		${LIBS_UHSP}
)




find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		RaccPPM
		OpenGL::GL
		glad
		glfw
)






