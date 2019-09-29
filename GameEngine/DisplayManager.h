#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

struct DisplayManager {
private:
	static double getTime();
	
	static double LAST_TIME;
public:
	static GLFWwindow* window;

	static int createDisplay(int width, int height);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void updateDisplay();
	static void closeDisplay();

	static void hideCursor();
	static void showCursor();

	static double DELTA;
};
