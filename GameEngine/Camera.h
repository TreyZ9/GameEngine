#pragma once

#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
public:
	static void move();
	static void move(float x, float y);
	static void updateViewMatrix();

	static glm::mat4 viewMatrix;
	static glm::vec3 position;
	static float pitch;
	static float roll;
	static float yaw;
};
