
#include "Camera.h"

#include "DisplayManager.h"
#include "Config.h"
#include "Maths.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

glm::mat4 Camera::viewMatrix = glm::mat4(1.0f);
glm::vec3 Camera::position = glm::vec3(0.0f, 1.6f, 0.0f);
float Camera::pitch = 0.0f;
float Camera::roll = 0.0f;
float Camera::yaw = 0.0f;

void Camera::move() 
{
	double xPos, yPos;
	glfwGetCursorPos(DisplayManager::window, &xPos, &yPos);
	glfwSetCursorPos(DisplayManager::window, Config::Display::WIDTH / 2, Config::Display::HEIGHT / 2);

	Camera::pitch += Config::Camera::LOOK_SPEED * (float)DisplayManager::DELTA * ((float)yPos - (Config::Display::HEIGHT / 2));
	Camera::yaw += Config::Camera::LOOK_SPEED * (float)DisplayManager::DELTA * ((float)xPos - (Config::Display::WIDTH / 2));

	if (Camera::pitch > Config::Camera::PITCH_MAX) { Camera::pitch = Config::Camera::PITCH_MAX; }
	if (Camera::pitch < Config::Camera::PITCH_MIN) { Camera::pitch = Config::Camera::PITCH_MIN; }

	float speedForward = 0.0f;
	float speedRight = 0.0f;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_W))
		speedForward += Config::Camera::MOVEMENT_SPEED * (float)DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_S))
		speedForward -= Config::Camera::MOVEMENT_SPEED * (float)DisplayManager::DELTA;

	if (glfwGetKey(DisplayManager::window, GLFW_KEY_D))
		speedRight += Config::Camera::MOVEMENT_SPEED * (float)DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_A))
		speedRight -= Config::Camera::MOVEMENT_SPEED * (float)DisplayManager::DELTA;

	if (Camera::yaw < 0.0f) { Camera::yaw += 360; }    // Overflow
	if (Camera::yaw > 360.0f) { Camera::yaw -= 360; }  // Protection

	position.x += (float)sin(Camera::yaw * 3.141592654f / 180.0f) * speedForward;
	position.z -= (float)cos(Camera::yaw * 3.141592654f / 180.0f) * speedForward;

	position.x += (float)sin((Camera::yaw + 90.0) * 3.141592654f / 180.0f) * speedRight;
	position.z -= (float)cos((Camera::yaw + 90.0) * 3.141592654f / 180.0f) * speedRight;

	if (glfwGetKey(DisplayManager::window, GLFW_KEY_SPACE))
		Camera::position.y += Config::Camera::MOVEMENT_SPEED * (float)DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_LEFT_SHIFT))
		Camera::position.y -= Config::Camera::MOVEMENT_SPEED * (float)DisplayManager::DELTA;

	Maths::createViewMatrix(Camera::viewMatrix);
}
