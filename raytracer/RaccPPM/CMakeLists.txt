project(RaccPPM)

add_library(${PROJECT_NAME})

target_sources( ${PROJECT_NAME}
	PUBLIC
		ppm.cpp
		ppm.hpp
)
