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

function(GENERATE_NEW_RT_TARGET executableName)
	RACC_DEFINE_EXECUTABLE_TARGET(${executableName})
	RACC_DEFINE_EXECUTABLE_TARGET(BM_${executableName})
endfunction()

# Naive
GENERATE_NEW_RT_TARGET(${PROJECT_NAME})

# With Binary BVH
set (RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH "RayTracer-TD_ES_BVH2")
GENERATE_NEW_RT_TARGET(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH})

# With BVH4
set (RT_BVH4_TOP_DOWN_MEDIAN_SPLIT "RayTracer-TD_ES_BVH4")
GENERATE_NEW_RT_TARGET(${RT_BVH4_TOP_DOWN_MEDIAN_SPLIT})

# With BVH8
set (RT_BVH8_TOP_DOWN_EVEN_SPLIT "RayTracer-TD_ES_BVH8")
GENERATE_NEW_RT_TARGET(${RT_BVH8_TOP_DOWN_EVEN_SPLIT})

# With BVH2 NAIVE SAH
set (RT_BVH2_TOP_DOWN_NAIVE_SAH "RayTracer-TD_NSAH_BVH2")
GENERATE_NEW_RT_TARGET(${RT_BVH2_TOP_DOWN_NAIVE_SAH})

# With BVH2 BOTTOM UP SAH
set (RT_BVH2_BOTTOM_UP_SAH "RayTracer-BU_SAH_BVH2")
GENERATE_NEW_RT_TARGET(${RT_BVH2_BOTTOM_UP_SAH})


# Also make our libraries available
add_subdirectory(RaccPPM)
add_subdirectory(RaccDebug)




#===========================================================
#	2. Sources
#-----------------------------------------------------------
set (THIRD_PARTY_UTILS_SRC
	"ThirdPartyUtils/RTIWUtils/RTIWConstants.hpp"
	"ThirdPartyUtils/benchmarkWrappedFunctions.cpp"
	"ThirdPartyUtils/benchmarkWrappedFunctions.hpp"
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
	"instrumenter.cpp"
	"instrumenter.hpp"
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


function(TARGET_LINK_FILES_COMPLETE projectName)
	TARGET_LINK_FILES(${projectName})
	TARGET_LINK_FILES(BM_${projectName})
endfunction()


function(TARGET_SELECT_BVH_ALGORITHM targetName selectedBVHAlgorithm)
	target_compile_definitions(${targetName}
		PRIVATE
			${selectedBVHAlgorithm}=1
	)

	target_compile_definitions(BM_${targetName}
		PRIVATE
			${selectedBVHAlgorithm}=1
			BENCHMARK_BUILD=1
	)
endfunction()





# RAY TRACER: NAIVE
TARGET_LINK_FILES_COMPLETE(${PROJECT_NAME})



# RAY TRACER: BINARY TOP DOWN MEDIAN SPLIT BVH
TARGET_LINK_FILES_COMPLETE(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH})
TARGET_SELECT_BVH_ALGORITHM(${RT_BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH} BINARY_TOP_DOWN_MEDIAN_SPLIT_BVH)



# RAY TRACER: BVH4 TOP DOWN EVEN SPLIT
TARGET_LINK_FILES_COMPLETE(${RT_BVH4_TOP_DOWN_MEDIAN_SPLIT})
TARGET_SELECT_BVH_ALGORITHM(${RT_BVH4_TOP_DOWN_MEDIAN_SPLIT} BVH4_TOP_DOWN_EVEN_SPLIT)



# RAY TRACER: BVH8 TOP DOWN EVEN SPLIT
TARGET_LINK_FILES_COMPLETE(${RT_BVH8_TOP_DOWN_EVEN_SPLIT})
TARGET_SELECT_BVH_ALGORITHM(${RT_BVH8_TOP_DOWN_EVEN_SPLIT} BVH8_TOP_DOWN_EVEN_SPLIT)



# RAY TRACER: BVH2 TOP DOWN NAIVE SAH
TARGET_LINK_FILES_COMPLETE(${RT_BVH2_TOP_DOWN_NAIVE_SAH})
TARGET_SELECT_BVH_ALGORITHM(${RT_BVH2_TOP_DOWN_NAIVE_SAH} BVH2_TOP_DOWN_NAIVE_SAH)



# RAY TRACER: BVH2 TOP DOWN NAIVE SAH
TARGET_LINK_FILES_COMPLETE(${RT_BVH2_BOTTOM_UP_SAH})
TARGET_SELECT_BVH_ALGORITHM(${RT_BVH2_BOTTOM_UP_SAH} BVH2_BOTTOM_UP_SAH)





