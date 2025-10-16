project(RaccDebug)

RACC_DEFINE_LIBRARY_TARGET(${PROJECT_NAME})

target_sources( ${PROJECT_NAME}
	PUBLIC
		RaccDebug.cpp
		RaccDebug.hpp
)
