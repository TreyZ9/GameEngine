#pragma once

#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>

#include <string>

class SkyboxShader : public ShaderProgram 
{
public:
	int location_projectionMatrix;
	int location_viewMatrix;

	SkyboxShader() = default;

	SkyboxShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	~SkyboxShader() = default;

	void bindAttributes();

	void getAllUniformLocations();

	void loadProjectionMatrix(const glm::mat4& matrix);

	void loadViewMatrix(const glm::mat4& viewMatrix);
};

