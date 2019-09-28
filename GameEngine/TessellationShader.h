#pragma once

#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "Maths.h"

class TessellationShader : public ShaderProgram {
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_eyePos;
	int location_lightPosition;
	int location_lightColor;
	int location_gamma;
	int location_blackPoint;

	// Default Constructor
	TessellationShader();
	// Constructor
	TessellationShader(std::string vertexShaderFilename, std::string fragmentShaderFilename, 
		std::string tessellationControlFilename, std::string tessellationEvaluationFilename);
	~TessellationShader();
	void bindAttributes();
	void getAllUniformLocations();
	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4);
	void loadViewMatrix();
	void loadCameraPosition(glm::vec3 pos);
	void loadLight(Light light);
	void loadGamma(float gamma);
	void loadBlackPoint(float blackPoint);
};

TessellationShader::TessellationShader() {}

TessellationShader::TessellationShader(std::string vertexShaderFilename, std::string fragmentShaderFilename, 
	std::string tessellationControlFilename, std::string tessellationEvaluationFilename)
	: ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename, tessellationControlFilename, 
	tessellationEvaluationFilename) { this->getAllUniformLocations(); }

TessellationShader::~TessellationShader() {}

void TessellationShader::bindAttributes() {
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
}

void TessellationShader::getAllUniformLocations() {
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
	this->location_eyePos = this->getUniformLocation("eyePos");
	this->location_lightPosition = this->getUniformLocation("lightPosition");
	this->location_lightColor = this->getUniformLocation("lightColor");
	this->location_gamma = this->getUniformLocation("gamma");
	this->location_blackPoint = this->getUniformLocation("blackPoint");
}

void TessellationShader::loadTransformationMatrix(glm::mat4 matrix) { this->loadMat4(this->location_transformationMatrix, matrix); }

void TessellationShader::loadProjectionMatrix(glm::mat4 matrix) { this->loadMat4(this->location_projectionMatrix, matrix); }

void TessellationShader::loadViewMatrix() {
	glm::mat4 view;
	Maths::createViewMatrix(view);
	this->loadMat4(this->location_viewMatrix, view);
}

void TessellationShader::loadCameraPosition(glm::vec3 pos) { this->loadVec3(this->location_eyePos, pos); }

void TessellationShader::loadLight(Light light) {
	this->loadVec3(this->location_lightPosition, light.getPosition());
	this->loadVec3(this->location_lightColor, light.getColor());
}

void TessellationShader::loadGamma(float gamma) { this->loadFloat(this->location_gamma, gamma); }

void TessellationShader::loadBlackPoint(float blackPoint) { this->loadFloat(this->location_blackPoint, blackPoint); }
