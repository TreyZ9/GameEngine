
#include "Shader.h"

#include "Camera.h"
#include "Maths.h"

Shader::Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) : 
	ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

void Shader::bindAttributes() 
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
	this->bindAttribute(3, "tangent");
	this->bindAttribute(4, "bitangent");
}

void Shader::getAllUniformLocations() 
{
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
}

void Shader::loadTransformationMatrix(const glm::mat4& matrix)
{ 
	this->loadMat4(this->location_transformationMatrix, matrix); 
}

void Shader::loadProjectionMatrix(const glm::mat4& matrix)
{ 
	this->loadMat4(this->location_projectionMatrix, matrix); 
}

void Shader::loadViewMatrix() 
{
	this->loadMat4(this->location_viewMatrix, Camera::viewMatrix);
}
