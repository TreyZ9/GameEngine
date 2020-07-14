#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct Maths {
	static void createTransformationMatrix(glm::mat4& mat, glm::vec3 translation, float rx, float ry, float rz, float scale);
	static void createViewMatrix(glm::mat4& mat);
	static void createViewMatrixAL(glm::mat4& mat);
};
