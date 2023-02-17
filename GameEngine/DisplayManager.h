#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include <string>
#include <map>

class Display
{
private:
	glm::mat4 projectionMatrix;
	glm::ivec2 resolution;
	double frameDelta = 0.016;
	double lastTime = 0.0;

	GLFWwindow* window;

public:
	Display(const unsigned int width, const unsigned int height, const std::string& title, GLFWwindow* parentWindow = NULL);
	~Display();

	void update();
	void clear();

	void hideCursor();
	void showCursor();

	glm::mat4 getProjectionMatrix() const;
	glm::ivec2 getResolution() const;
	GLFWwindow* getWindow() const;
	double getFrameDelta() const;

	void setResolution(const unsigned int width, const unsigned int height);
};

struct DisplayManager {
private:
	static std::map<GLFWwindow*, Display*> displays;

public:
	static void addDisplay(Display& display);
	static void removeDisplay(const Display& display);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};