cmake_minimum_required(VERSION 3.30)

project(RayTracer)

set(RACC_COMMON_CMAKE_UTILS ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(RACC_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)


include(${RACC_COMMON_CMAKE_UTILS}/NoInSourceBuild.cmake)
add_subdirectory(${RACC_COMMON_CMAKE_UTILS}/RaccProjectCMake)

add_subdirectory(raytracer)
add_subdirectory(libs)





