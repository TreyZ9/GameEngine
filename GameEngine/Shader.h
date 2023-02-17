#pragma once

#include "ShaderProgram.h"

class Shader : public ShaderProgram 
{
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;

	Shader() = default;

	Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	~Shader() = default;

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(const glm::mat4& matrix);

	void loadProjectionMatrix(const glm::mat4& matrix);

	void loadViewMatrix();
};
