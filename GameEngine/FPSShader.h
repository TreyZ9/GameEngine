#pragma once

#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>

class FPSShader : public ShaderProgram 
{
private:
	std::string VERTEX_SHADER_FILENAME = "vertexShaderFPS.vert";
	std::string FRAGMENT_SHADER_FILENAME = "fragmentShaderFPS.frag";
public:
	int location_transformationMatrix;

	FPSShader();

	FPSShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	~FPSShader();

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(glm::mat4 matrix);
};
