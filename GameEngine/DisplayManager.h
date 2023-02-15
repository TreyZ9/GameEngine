#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

struct DisplayManager {
private:
	static glm::mat4 projectionMatrix;
	static glm::ivec2 resolution;
	static double frameDelta;
	static double lastTime;

	static double getTime();

public:
	static GLFWwindow* window;

	static int createDisplay(int width, int height);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void updateDisplay();
	static void clearScreenBuffer();
	static void closeDisplay();
	static void hideCursor();
	static void showCursor();
	static glm::mat4 getProjectionMatrix();
	static glm::ivec2 getResolution();
	static double getFrameDelta();
};
