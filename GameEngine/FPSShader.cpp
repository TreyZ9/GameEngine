
#include "FPSShader.h"

FPSShader::FPSShader() {}

FPSShader::FPSShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

FPSShader::~FPSShader() {}

void FPSShader::bindAttributes() 
{
	this->bindAttribute(0, "vertex");
}

void FPSShader::getAllUniformLocations() 
{
	this->location_transformationMatrix = this->getUniformLocation("projectionMatrix");
	this->location_textColor = this->getUniformLocation("textColor");
}

void FPSShader::loadTransformationMatrix(glm::mat4 matrix) 
{ 
	this->loadMat4(this->location_transformationMatrix, matrix); 
}

void FPSShader::loadTextColor(glm::vec3 color)
{
	this->loadVec3(this->location_textColor, color);
}
