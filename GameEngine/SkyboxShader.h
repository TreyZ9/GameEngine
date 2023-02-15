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

	SkyboxShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	~SkyboxShader() = default;

	void bindAttributes();

	void getAllUniformLocations();

	void loadProjectionMatrix(glm::mat4);

	void loadViewMatrix(glm::mat4 viewMatrix);
};

