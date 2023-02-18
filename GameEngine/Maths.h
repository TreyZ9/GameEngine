#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Maths 
{
	void createTransformationMatrix(glm::mat4& mat, const glm::vec3& translation, const float rx, const float ry, const float rz, const float scale);

	void createViewMatrix(glm::mat4& mat);

	void createViewMatrixAL(glm::mat4& mat);
};
