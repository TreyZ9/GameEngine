
#include "Light.h"

Light::Light() {}

Light::Light(glm::vec3 position, glm::vec3 color) : position(position), color(color) {}

Light::~Light() {}

// Getters
glm::vec3 Light::getPosition() { return this->position; }
glm::vec3 Light::getColor() { return this->color; }

// Setters;
void Light::setPosition(glm::vec3 position) { this->position = position; }
void Light::setColor(glm::vec3 color) { this->color = color; }
