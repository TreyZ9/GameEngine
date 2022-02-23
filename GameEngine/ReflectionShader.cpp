
#include "ReflectionShader.h"

#include "Camera.h"
#include "Maths.h"

ReflectionShader::ReflectionShader() 
{
	this->location_transformationMatrix = NULL;
	this->location_projectionMatrix = NULL;
	this->location_viewMatrix = NULL;
	this->location_lightPosition = NULL;
	this->location_lightColor = NULL;
	this->location_gamma = NULL;
	this->location_useSpecularMap = NULL;
	this->location_cameraPosition = NULL;
}

ReflectionShader::ReflectionShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

ReflectionShader::~ReflectionShader() { }

void ReflectionShader::bindAttributes() 
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
	this->bindAttribute(3, "tangent");
	this->bindAttribute(4, "bitangent");
}

void ReflectionShader::getAllUniformLocations() 
{
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
	this->location_lightPosition = this->getUniformLocation("lightPosition");
	this->location_lightColor = this->getUniformLocation("lightColor");
	this->location_gamma = this->getUniformLocation("gamma");
	this->location_cameraPosition = this->getUniformLocation("cameraPosition");
	this->location_useSpecularMap = this->getUniformLocation("useSpecularMap");
}

void ReflectionShader::loadTransformationMatrix(glm::mat4 matrix) 
{ 
	this->loadMat4(this->location_transformationMatrix, matrix); 
}

void ReflectionShader::loadProjectionMatrix(glm::mat4 matrix) 
{ 
	this->loadMat4(this->location_projectionMatrix, matrix); 
}

void ReflectionShader::loadViewMatrix()
{
	this->loadMat4(this->location_viewMatrix, Camera::viewMatrix);
}

void ReflectionShader::loadLight(Light light)
{
	this->loadVec3(this->location_lightPosition, light.position);
	this->loadVec3(this->location_lightColor, light.color);
}

void ReflectionShader::loadGamma(float gamma)
{
	this->loadFloat(this->location_gamma, gamma);
}

void ReflectionShader::setUseSpecularMap(bool useSpecularMap)
{
	this->loadBoolean(this->location_useSpecularMap, useSpecularMap);
}

void ReflectionShader::loadCameraPosition(glm::vec3 cameraPosition) 
{ 
	this->loadVec3(this->location_cameraPosition, cameraPosition); 
}

