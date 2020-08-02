
#include "DisplayManager.h"

#include "Config.h"

#include <iostream>

double DisplayManager::LAST_TIME = DisplayManager::getTime();
double DisplayManager::DELTA = 0.1;

GLFWwindow* DisplayManager::window;

double DisplayManager::getTime() { return glfwGetTime(); }

int DisplayManager::createDisplay(int width, int height) {
	Config::Display::WIDTH = width;
	Config::Display::HEIGHT = height;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::Display::OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::Display::OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	GLFWwindow* window = glfwCreateWindow(Config::Display::WIDTH, Config::Display::HEIGHT, Config::Display::TITLE.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "could not create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "could not load glad" << std::endl;
		return -1;
	}

	glViewport(0, 0, Config::Display::WIDTH, Config::Display::HEIGHT);
	glfwSetFramebufferSizeCallback(window, DisplayManager::framebuffer_size_callback);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	DisplayManager::window = window;

	glfwSetCursorPos(DisplayManager::window, Config::Display::WIDTH / 2, Config::Display::HEIGHT / 2);

	return 0;
}

void DisplayManager::updateDisplay() {
	glfwSwapBuffers(DisplayManager::window);
	glfwPollEvents();

	double thisTime = DisplayManager::getTime();
	DisplayManager::DELTA = thisTime - DisplayManager::LAST_TIME;
	DisplayManager::LAST_TIME = thisTime;
}

void DisplayManager::closeDisplay() { glfwTerminate(); }

void DisplayManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Config::Display::WIDTH = width;
	Config::Display::HEIGHT = height;
}

void DisplayManager::hideCursor() { glfwSetInputMode(DisplayManager::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

void DisplayManager::showCursor() { glfwSetInputMode(DisplayManager::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
