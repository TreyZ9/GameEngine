#pragma once

#include <glm\glm\common.hpp>

class Light {
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

Light::Light() {}

Light::Light(glm::vec3 position, glm::vec3 color) {
	this->position = position;
	this->color = color;
}

Light::~Light() {}

// Getters
glm::vec3 Light::getPosition() { return this->position; }
glm::vec3 Light::getColor() { return this->color; }

// Setters;
void Light::setPosition(glm::vec3 position) { this->position = position; }
void Light::setColor(glm::vec3 color) { this->color = color; }
