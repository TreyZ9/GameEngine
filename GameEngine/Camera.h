#pragma once

#include <glm/common.hpp>

struct Camera {
public:
	static void move();
	static void move(float x, float y);

	static glm::vec3 position;
	static float pitch;
	static float roll;
	static float yaw;
};
