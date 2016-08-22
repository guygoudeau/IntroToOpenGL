#include "MyApplication.h"

Application::Application()
{
	white = vec4(1, 1, 1, 1);
	yellow = vec4(1, 0.8, 0.1, 1);
	green = vec4(0.1, 0.6, 0.1, 1);
}

bool Application::startup()
{

	if (glfwInit() == false)
		return false;

	window = glfwCreateWindow(1280, 720, "The Milky Way", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	Gizmos::create(); // initializes

	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0)); // camera location
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f); // camera lens

	auto major = ogl_GetMajorVersion(); // Gets what version of openGL this program is running
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0.0f, .0f, 0.0f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	return true;
}

bool Application::update()
{
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		angle += .03f; // angle by which to rotate. can slow down or speed up by changing this

		mat4 sunRotation = glm::rotate(angle, glm::vec3(0, 1, 0)); // tell sun to rotate by an angle
		sun = mat4(1) * sunRotation; // sun stays rotating in place

		mat4 earthTransform = glm::translate(earthOffset); // tells earth to orbit sun relative to position
		earth = sun * earthTransform * glm::rotate(angle, glm::vec3(0, 1, 0)); // makes sun move and rotate

		mat4 moonTransform = glm::translate(moonOffset); // tells moon to orbit earth relative to position
		moon = earth * moonTransform; // makes moon move

		return true;
	}
	return false;
}

void Application::draw()
{
	Gizmos::addSphere(vec3(sun[3]), 3, 30, 30, yellow, &sun); // drwa sun
	Gizmos::addSphere(vec3(earth[3]), 1, 30, 30, green, &earth); // draw earth
	Gizmos::addSphere(vec3(moon[3]), .5, 30, 30, white, &moon); // draw moon

	Gizmos::draw(projection * view);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Application::shutdown()
{
	Gizmos::destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}
