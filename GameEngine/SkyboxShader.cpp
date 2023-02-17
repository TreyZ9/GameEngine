
#include "SkyboxShader.h"

#include "Camera.h"
#include "Maths.h"

SkyboxShader::SkyboxShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) : 
	ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

void SkyboxShader::bindAttributes() 
{
	this->bindAttribute(0, "position");
}

void SkyboxShader::getAllUniformLocations() 
{
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
}

void SkyboxShader::loadProjectionMatrix(const glm::mat4& matrix)
{ 
	this->loadMat4(this->location_projectionMatrix, matrix); 
}

void SkyboxShader::loadViewMatrix(const glm::mat4& viewMatrix)
{
	this->loadMat4(this->location_viewMatrix, viewMatrix);
}
