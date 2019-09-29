
#include "Maths.h"

#include "Camera.h"

void Maths::createTransformationMatrix(glm::mat4& mat, glm::vec3 translation, float rx, float ry, float rz, float scale) {
	mat = glm::mat4(1.0f);
	mat = glm::translate(mat, translation);
	mat = glm::rotate(mat, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::scale(mat, glm::vec3(scale));
}

void Maths::createViewMatrix(glm::mat4& mat) {
	mat = glm::mat4(1.0f);
	mat = glm::rotate(mat, glm::radians(Camera::pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(Camera::yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(Camera::roll), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::translate(mat, -Camera::position);
}
