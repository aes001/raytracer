project(stb-image)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
	PRIVATE
		"./src/stb_image.cpp"
		"./src/stb_image_write.cpp"
		"./include/stb_image.h"
		"./include/stb_image_write.h"
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		"./include"
)
