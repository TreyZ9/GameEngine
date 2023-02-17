
#include "DisplayManager.h"

#include <spdlog/spdlog.h>

#include "OpenGLFunctions.h"
#include "Config.h"

glm::mat4 DisplayManager::projectionMatrix = glm::mat4(1.0f);
glm::ivec2 DisplayManager::resolution = glm::ivec2(0.0f);
double DisplayManager::lastTime = 0.0f;
double DisplayManager::frameDelta = 0.1f;

GLFWwindow* DisplayManager::window;

int DisplayManager::createDisplay(const int width, const int height)
{
	DisplayManager::resolution = glm::ivec2(width, height);
	DisplayManager::projectionMatrix = glm::perspective(
		Config::Display::FOV,
		(float)DisplayManager::resolution.x / (float)DisplayManager::resolution.y,
		Config::Display::NEAR_PLANE,
		Config::Display::FAR_PLANE);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::Display::OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::Display::OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	GLFWwindow* window = glfwCreateWindow(DisplayManager::resolution.x, DisplayManager::resolution.y, Config::Display::TITLE.c_str(), NULL, NULL);
	if (window == NULL) 
	{
		spdlog::error("Failed to create GLFW window with size {:d}x{:d}", DisplayManager::resolution.x, DisplayManager::resolution.y);
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		spdlog::error("Failed to load glad");
		return -1;
	}

	glCall(glViewport, 0, 0, DisplayManager::resolution.x, DisplayManager::resolution.y);
	glfwSetFramebufferSizeCallback(window, DisplayManager::framebuffer_size_callback);

	glCall(glEnable, GL_CULL_FACE);
	glCall(glCullFace, GL_BACK);
	glCall(glEnable, GL_DEPTH_TEST);

	DisplayManager::window = window;

	glfwSetCursorPos(DisplayManager::window, DisplayManager::resolution.x / 2, DisplayManager::resolution.y / 2);

	return 0;
}

void DisplayManager::closeDisplay()
{
	glfwTerminate();
}

void DisplayManager::updateDisplay() 
{
	glfwSwapBuffers(DisplayManager::window);
	glfwPollEvents();

	double thisTime = glfwGetTime();
	DisplayManager::frameDelta = thisTime - DisplayManager::lastTime;
	DisplayManager::lastTime = thisTime;
}

void DisplayManager::clearScreenBuffer()
{
	glCall(glClearColor, 0.0f, 1.0f, 1.0f, 1.0f);
	glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DisplayManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glCall(glViewport, 0, 0, width, height);
	DisplayManager::resolution.x = width;
	DisplayManager::resolution.y = height;
	DisplayManager::projectionMatrix = glm::perspective(
		Config::Display::FOV, 
		(float)DisplayManager::resolution.x / (float)DisplayManager::resolution.y, 
		Config::Display::NEAR_PLANE, 
		Config::Display::FAR_PLANE);
}

void DisplayManager::hideCursor() 
{ 
	glfwSetInputMode(DisplayManager::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
}

void DisplayManager::showCursor() 
{ 
	glfwSetInputMode(DisplayManager::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
}

glm::mat4 DisplayManager::getProjectionMatrix()
{
	return DisplayManager::projectionMatrix;
}

glm::ivec2 DisplayManager::getResolution()
{
	return DisplayManager::resolution;
}

double DisplayManager::getFrameDelta()
{
	return DisplayManager::frameDelta;
}