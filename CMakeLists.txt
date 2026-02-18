cmake_minimum_required(VERSION 3.30)

project(RayTracer)

set(CMAKE_CXX_STANDARD 23)

set(RACC_COMMON_CMAKE_UTILS ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(RACC_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)


include(${RACC_COMMON_CMAKE_UTILS}/NoInSourceBuild.cmake)
add_subdirectory(${RACC_COMMON_CMAKE_UTILS}/RaccProjectCMake)

add_subdirectory(libs)
add_subdirectory(raytracer)





# Note for me only:
# cmake -B . -S ../../raytracer/ \
#		-DCMAKE_BUILD_TYPE=debug \
#		-DCMAKE_C_COMPILER=clang \
#		-DCMAKE_CXX_COMPILER=clang++ \
#		-DCMAKE_CXX_FLAGS_INIT="-stdlib=libc++"
#		-DCMAKE_EXE_LINKER_FLAGS_INIT="-stdlib=libc++"
#		-G Ninja





