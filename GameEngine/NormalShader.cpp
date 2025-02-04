
#include "NormalShader.h"

#include "Camera.h"
#include "Maths.h"

NormalShader::NormalShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

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

void NormalShader::loadTransformationMatrix(const glm::mat4& matrix)
{ 
	this->loadMat4(this->location_transformationMatrix, matrix); 
}

void NormalShader::loadProjectionMatrix(const glm::mat4& matrix)
{ 
	this->loadMat4(this->location_projectionMatrix, matrix); 
}

void NormalShader::loadViewMatrix()
{
	this->loadMat4(this->location_viewMatrix, Camera::viewMatrix);
}

void NormalShader::loadLight(const Light& light)
{
	this->loadVec3(this->location_lightPosition, light.position);
	this->loadVec3(this->location_lightColor, light.color);
}

void NormalShader::loadGamma(const float gamma)
{
	this->loadFloat(this->location_gamma, gamma);
}

void NormalShader::loadCameraPosition(const glm::vec3& cameraPosition)
{ 
	this->loadVec3(this->location_cameraPosition, cameraPosition); 
}

