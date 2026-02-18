#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ppm.hpp"
#include "RTIWConglomerate.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "triangle.hpp"





void processInput(GLFWwindow *window);





int main()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "Failed to initialized GLFW\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Ray Tracer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
		char const * msg = nullptr;
		int ecode = glfwGetError(&msg);
		std::cout << "Error code: " << ecode << "\n";
		std::cout << "Error: " << msg << "\n";
		glfwTerminate();
		return -1;
    }
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	auto frameBufferSizeCallback = [](GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); };

    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	// World
	RTIW::hittable_list world;

	// world.add(std::make_shared<RTIW::sphere>(RTIW::point3(0, 0, -1), 0.5));
	world.add(std::make_shared<RTIW::sphere>(RTIW::point3(0,-100.5,-1), 100));
	// world.add(std::make_shared<RTIW::Triangle>(RTIW::vec3(0, 0.5, -1),
	// 										   RTIW::vec3(-0.5, -0.5, -1),
	// 										   RTIW::vec3(0.5, -0.5, -1.5)));

	world.add(std::make_shared<RTIW::Triangle>(RTIW::vec3(0, 0.5, -1),
											   RTIW::vec3(-0.05, -0.5, -1.5),
											   RTIW::vec3(0.05, -0.5, -0.5)));


	const double aspectRatio = 16.0 / 9.0;
	const int imageWidth = 1280;
	const int sampleCount = 100;

	RTIW::camera cam;
	cam.SetAspectRatio(aspectRatio);
	cam.SetWidth(imageWidth);
	cam.SetSampleCount(sampleCount);

	auto image = RACCPPM::PPMMaker::NewPPMImage("OutputImage",
												imageWidth,
												aspectRatio);

	cam.Render(world, image);

	image.SaveAs("OutputImage");

	glfwTerminate();
	return 0;
}




void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
