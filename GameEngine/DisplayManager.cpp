
#include "DisplayManager.h"

#include <spdlog/spdlog.h>

#include "OpenGLFunctions.h"
#include "Config.h"


// ---------- Display ----------
Display::Display(const unsigned int width, const unsigned int height, const std::string& title, GLFWwindow* parentWindow)
{
	this->setResolution(width, height);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::Display::OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::Display::OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	GLFWwindow* window = glfwCreateWindow(this->resolution.x, this->resolution.y, title.c_str(), NULL, parentWindow);
	if (window == NULL)
	{
		spdlog::error("Failed to create GLFW window with size {:d}x{:d}", this->resolution.x, this->resolution.y);
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error("Failed to load glad");
	}

	glCall(glViewport, 0, 0, this->resolution.x, this->resolution.y);
	glfwSetFramebufferSizeCallback(window, DisplayManager::framebuffer_size_callback);

	glCall(glEnable, GL_CULL_FACE);
	glCall(glCullFace, GL_BACK);
	glCall(glEnable, GL_DEPTH_TEST);

	this->window = window;

	glfwSetCursorPos(this->window, this->resolution.x / 2, this->resolution.y / 2);

	DisplayManager::addDisplay(*this);
}

Display::~Display()
{
	glfwTerminate();
}

void Display::update()
{
	glfwSwapBuffers(this->window);
	glfwPollEvents();

	double thisTime = glfwGetTime();
	this->frameDelta = thisTime - this->lastTime;
	this->lastTime = thisTime;
}

void Display::clear()
{
	glCall(glClearColor, 0.0f, 1.0f, 1.0f, 1.0f);
	glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::hideCursor()
{
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Display::showCursor()
{
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

glm::mat4 Display::getProjectionMatrix() const
{
	return this->projectionMatrix;
}

glm::ivec2 Display::getResolution() const
{
	return this->resolution;
}

GLFWwindow* Display::getWindow() const
{
	return this->window;
}

double Display::getFrameDelta() const
{
	return this->frameDelta;
}

void Display::setResolution(const unsigned int width, const unsigned int height)
{
	this->resolution.x = width;
	this->resolution.y = height;
	this->projectionMatrix = glm::perspective(
		Config::Display::FOV,
		(float)this->resolution.x / (float)this->resolution.y,
		Config::Display::NEAR_PLANE,
		Config::Display::FAR_PLANE);
}


// ---------- Display Manager ----------
std::map<GLFWwindow*, Display*> DisplayManager::displays;

void DisplayManager::addDisplay(Display& display)
{
	GLFWwindow* window = display.getWindow();
	if (DisplayManager::displays.find(window) == DisplayManager::displays.end())
	{
		DisplayManager::displays.insert(std::pair<GLFWwindow*, Display*>(window, &display));
	}
}

void DisplayManager::removeDisplay(const Display& display)
{
	GLFWwindow* window = display.getWindow();
	if (DisplayManager::displays.find(window) != DisplayManager::displays.end())
	{
		DisplayManager::displays.erase(window);
	}
}

void DisplayManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (DisplayManager::displays.find(window) != DisplayManager::displays.end())
	{
		Display* display = DisplayManager::displays.at(window);
		glCall(glViewport, 0, 0, width, height); // <-------------- Separate into window start functions
		display->setResolution(width, height);
	}
	else
	{
		spdlog::error("Resized a window not registered in display manager");
	}
}