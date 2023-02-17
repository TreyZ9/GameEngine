#pragma once

#include "DisplayManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

struct Camera 
{
public:
	static glm::mat4 viewMatrix;
	static glm::vec3 position;
	static glm::vec3 rotation;

	static void move(const Display& display);
	static void update();
};
