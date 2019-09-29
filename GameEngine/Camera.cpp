
#include "Camera.h"

#include "DisplayManager.h"
#include "Config.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

glm::vec3 Camera::position = glm::vec3(1.0f);
float Camera::pitch = 0.0f;
float Camera::roll = 0.0f;
float Camera::yaw = 0.0f;

void Camera::move() 
{
	float speedForward = 0.0f;
	float speedRight = 0.0f;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_W))
		speedForward += Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_S))
		speedForward -= Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;

	if (glfwGetKey(DisplayManager::window, GLFW_KEY_D))
		speedRight += Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_A))
		speedRight -= Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;

	if (Camera::yaw < 0.0f) { Camera::yaw += 360; }    // Overflow
	if (Camera::yaw > 360.0f) { Camera::yaw -= 360; }  // Protection

	position.x += sin(Camera::yaw * 3.141592654 / 180) * speedForward;
	position.z -= cos(Camera::yaw * 3.141592654 / 180) * speedForward;

	position.x += sin((Camera::yaw + 90.0) * 3.141592654 / 180) * speedRight;
	position.z -= cos((Camera::yaw + 90.0) * 3.141592654 / 180) * speedRight;

	if (glfwGetKey(DisplayManager::window, GLFW_KEY_SPACE))
		Camera::position.y += Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_LEFT_SHIFT))
		Camera::position.y -= Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;

	if (glfwGetKey(DisplayManager::window, GLFW_KEY_UP))
		Camera::pitch -= Config::Camera::LOOK_SPEED * DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_DOWN))
		Camera::pitch += Config::Camera::LOOK_SPEED * DisplayManager::DELTA;

	if (glfwGetKey(DisplayManager::window, GLFW_KEY_LEFT))
		Camera::yaw -= Config::Camera::LOOK_SPEED * DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_RIGHT))
		Camera::yaw += Config::Camera::LOOK_SPEED * DisplayManager::DELTA;
}

void Camera::move(float x, float y) 
{
	float speedForward = 0.0f;
	float speedRight = 0.0f;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_W))
		speedForward += Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_S))
		speedForward -= Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;

	if (glfwGetKey(DisplayManager::window, GLFW_KEY_D))
		speedRight += Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_A))
		speedRight -= Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;

	if (Camera::yaw < 0.0f) { Camera::yaw += 360; }    // Overflow
	if (Camera::yaw > 360.0f) { Camera::yaw -= 360; }  // Protection

	position.x += sin(Camera::yaw * 3.141592654 / 180) * speedForward;
	position.z -= cos(Camera::yaw * 3.141592654 / 180) * speedForward;

	position.x += sin((Camera::yaw + 90.0f) * 3.141592654 / 180) * speedRight;
	position.z -= cos((Camera::yaw + 90.0f) * 3.141592654 / 180) * speedRight;

	if (glfwGetKey(DisplayManager::window, GLFW_KEY_SPACE))
		Camera::position.y += Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;
	if (glfwGetKey(DisplayManager::window, GLFW_KEY_LEFT_SHIFT))
		Camera::position.y -= Config::Camera::MOVEMENT_SPEED * DisplayManager::DELTA;

	Camera::pitch += Config::Camera::LOOK_SPEED * DisplayManager::DELTA * y;
	Camera::yaw += Config::Camera::LOOK_SPEED * DisplayManager::DELTA * x;

	if (Camera::pitch > Config::Camera::PITCH_MAX) { Camera::pitch = Config::Camera::PITCH_MAX; }
	if (Camera::pitch < Config::Camera::PITCH_MIN) { Camera::pitch = Config::Camera::PITCH_MIN; }
}

