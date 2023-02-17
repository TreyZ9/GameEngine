#pragma once

#include <glm/common.hpp>

class Light 
{
public:
	glm::vec3 position;
	glm::vec3 color;

	Light(const glm::vec3& position, const glm::vec3& color);

	~Light() = default;
};
