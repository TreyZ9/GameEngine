#pragma once

#include <glm/common.hpp>

class Light 
{
private:
	glm::vec3 position;
	glm::vec3 color;

public:
	Light();
	Light(glm::vec3 position, glm::vec3 color);
	~Light();

	// Getters
	glm::vec3 getPosition();
	glm::vec3 getColor();

	// Setters
	void setPosition(glm::vec3 position);
	void setColor(glm::vec3 color);
};

