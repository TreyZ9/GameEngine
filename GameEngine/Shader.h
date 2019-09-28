#pragma once

#include "ShaderProgram.h"
#include "Camera.h"
#include "Maths.h"

class Shader : public ShaderProgram {
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;

	// Default Constructor
	Shader();
	// Constructor
	Shader(std::string vertexShaderFilename, std::string fragmentShaderFilename);
	~Shader();
	void bindAttributes();
	void getAllUniformLocations();
	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4);
	void loadViewMatrix();
};

Shader::Shader() {}

Shader::Shader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename) { this->getAllUniformLocations(); }

Shader::~Shader() {}

void Shader::bindAttributes() {
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
	this->bindAttribute(3, "tangent");
	this->bindAttribute(4, "bitangent");
}

void Shader::getAllUniformLocations() {
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
}

void Shader::loadTransformationMatrix(glm::mat4 matrix) { this->loadMat4(this->location_transformationMatrix, matrix); }

void Shader::loadProjectionMatrix(glm::mat4 matrix) { this->loadMat4(this->location_projectionMatrix, matrix); }

void Shader::loadViewMatrix() {
	glm::mat4 view;
	Maths::createViewMatrix(view);
	this->loadMat4(this->location_viewMatrix, view);
}
