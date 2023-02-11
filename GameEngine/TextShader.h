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

	TextShader();

	TextShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	~TextShader();

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(glm::mat4 matrix);

	void loadProjectionMatrix(glm::mat4 matrix);

	void loadViewMatrix(glm::mat4 matrix);

	void loadTextColor(glm::vec3 color);
};
