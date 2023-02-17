#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct Maths 
{
	static void createTransformationMatrix(glm::mat4& mat, const glm::vec3& translation, const float rx, const float ry, const float rz, const float scale);

	static void createViewMatrix(glm::mat4& mat);

	static void createViewMatrixAL(glm::mat4& mat);
};
