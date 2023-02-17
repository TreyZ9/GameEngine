#pragma once

#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>

class TextShader : public ShaderProgram 
{
private:
	std::string VERTEX_SHADER_FILENAME = "vertexShaderFPS.vert";
	std::string FRAGMENT_SHADER_FILENAME = "fragmentShaderFPS.frag";
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_textColor;

	TextShader() = default;

	TextShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	~TextShader() = default;

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(const glm::mat4& matrix);

	void loadProjectionMatrix(const glm::mat4& matrix);

	void loadViewMatrix(const glm::mat4& matrix);

	void loadTextColor(const glm::vec3& color);
};
