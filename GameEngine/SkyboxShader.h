#pragma once

#include "ShaderProgram.h"
#include "Camera.h"
#include "Maths.h"

class SkyboxShader : public ShaderProgram {
public:
	int location_projectionMatrix;
	int location_viewMatrix;

	// Default Constructor
	SkyboxShader();
	// Constructor
	SkyboxShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);
	~SkyboxShader();
	void bindAttributes();
	void getAllUniformLocations();
	void loadProjectionMatrix(glm::mat4);
	void loadViewMatrix(glm::mat4 viewMatrix);
};

SkyboxShader::SkyboxShader() {}

SkyboxShader::SkyboxShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename) { this->getAllUniformLocations(); }

SkyboxShader::~SkyboxShader() {}

void SkyboxShader::bindAttributes() {
	this->bindAttribute(0, "position");
}

void SkyboxShader::getAllUniformLocations() {
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
}

void SkyboxShader::loadProjectionMatrix(glm::mat4 matrix) { this->loadMat4(this->location_projectionMatrix, matrix); }

void SkyboxShader::loadViewMatrix(glm::mat4 viewMatrix) {
	this->loadMat4(this->location_viewMatrix, viewMatrix);
}
