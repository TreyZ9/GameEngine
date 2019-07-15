#pragma once

#include <string>

#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "Maths.h"

class FPSShader : public ShaderProgram {
private:
	std::string VERTEX_SHADER_FILENAME = "vertexShaderFPS.vert";
	std::string FRAGMENT_SHADER_FILENAME = "fragmentShaderFPS.frag";
public:
	int location_transformationMatrix;

	// Default Constructor
	FPSShader();
	// Constructor
	FPSShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);
	~FPSShader();
	void bindAttributes();
	void getAllUniformLocations();
	void loadTransformationMatrix(glm::mat4 matrix);
};

FPSShader::FPSShader() {}

FPSShader::FPSShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename) { this->getAllUniformLocations(); }

FPSShader::~FPSShader() {}

void FPSShader::bindAttributes() {
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void FPSShader::getAllUniformLocations() {
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
}

void FPSShader::loadTransformationMatrix(glm::mat4 matrix) { this->loadMat4(this->location_transformationMatrix, matrix); }