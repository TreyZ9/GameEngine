#pragma once

#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "Maths.h"

class NormalShader : public ShaderProgram {
private:
	std::string VERTEX_SHADER_FILENAME = "vertexShader.vert";
	std::string FRAGMENT_SHADER_FILENAME = "fragmentShader.frag";
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_lightPosition;
	int location_lightColor;
	int location_gamma;
	int location_cameraPosition;

	// Default Constructor
	NormalShader();
	// Constructor
	NormalShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);
	~NormalShader();
	void bindAttributes();
	void getAllUniformLocations();
	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4);
	void loadViewMatrix();
	void loadLight(Light light);
	void loadGamma(float gamma);
	void loadCameraPosition(glm::vec3 cameraPositon);
};

NormalShader::NormalShader() {}

NormalShader::NormalShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename) { this->getAllUniformLocations(); }

NormalShader::~NormalShader() {}

void NormalShader::bindAttributes() {
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
	this->bindAttribute(3, "tangent");
	this->bindAttribute(4, "bitangent");
}

void NormalShader::getAllUniformLocations() {
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
	this->location_lightPosition = this->getUniformLocation("lightPosition");
	this->location_lightColor = this->getUniformLocation("lightColor");
	this->location_gamma = this->getUniformLocation("gamma");
	this->location_cameraPosition = this->getUniformLocation("cameraPosition");
}

void NormalShader::loadTransformationMatrix(glm::mat4 matrix) { this->loadMat4(this->location_transformationMatrix, matrix); }

void NormalShader::loadProjectionMatrix(glm::mat4 matrix) { this->loadMat4(this->location_projectionMatrix, matrix); }

void NormalShader::loadViewMatrix()
{
	glm::mat4 view;
	Maths::createViewMatrix(view);
	this->loadMat4(this->location_viewMatrix, view);
}

void NormalShader::loadLight(Light light)
{
	this->loadVec3(this->location_lightPosition, light.getPosition());
	this->loadVec3(this->location_lightColor, light.getColor());
}

void NormalShader::loadGamma(float gamma)
{
	this->loadFloat(this->location_gamma, gamma);
}

void NormalShader::loadCameraPosition(glm::vec3 cameraPosition) { this->loadVec3(this->location_cameraPosition, cameraPosition); }
