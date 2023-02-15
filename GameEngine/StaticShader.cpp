
#include "StaticShader.h"

#include "Camera.h"
#include "Maths.h"

StaticShader::StaticShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

void StaticShader::bindAttributes() 
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
	this->bindAttribute(3, "tangent");
	this->bindAttribute(4, "bitangent");
}

void StaticShader::getAllUniformLocations() 
{
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
	this->location_lightPosition = this->getUniformLocation("lightPosition");
	this->location_lightColor = this->getUniformLocation("lightColor");
	this->location_gamma = this->getUniformLocation("gamma");
}

void StaticShader::loadTransformationMatrix(glm::mat4 matrix) 
{ 
	this->loadMat4(this->location_transformationMatrix, matrix); 
}

void StaticShader::loadProjectionMatrix(glm::mat4 matrix) 
{ 
	this->loadMat4(this->location_projectionMatrix, matrix); 
}

void StaticShader::loadViewMatrix()
{
	this->loadMat4(this->location_viewMatrix, Camera::viewMatrix);
}

void StaticShader::loadLight(Light light)
{
	this->loadVec3(this->location_lightPosition, light.position);
	this->loadVec3(this->location_lightColor, light.color);
}

void StaticShader::loadGamma(float gamma)
{
	this->loadFloat(this->location_gamma, gamma);
}
