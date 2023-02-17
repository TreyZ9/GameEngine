
#include "Maths.h"

#include "Camera.h"

void Maths::createTransformationMatrix(glm::mat4& mat, const glm::vec3& translation, const float rx, const float ry, const float rz, const float scale)
{
	mat = glm::mat4(1.0f);
	mat = glm::translate(mat, translation);
	mat = glm::rotate(mat, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::scale(mat, glm::vec3(scale));
}

void Maths::createViewMatrix(glm::mat4& mat) 
{
	mat = glm::mat4(1.0f);
	mat = glm::rotate(mat, glm::radians(Camera::rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(Camera::rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(Camera::rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::translate(mat, -Camera::position);
}

void Maths::createViewMatrixAL(glm::mat4& mat)
{
	float yaw = Camera::rotation.y + 180.0f;
	mat = glm::mat4(1.0f);
	mat = glm::rotate(mat, glm::radians(Camera::rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(Camera::rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}
