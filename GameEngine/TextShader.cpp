
#include "TextShader.h"

TextShader::TextShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

void TextShader::bindAttributes()
{
	this->bindAttribute(0, "vertex_vs");
}

void TextShader::getAllUniformLocations()
{
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
	this->location_textColor = this->getUniformLocation("textColor");
}

void TextShader::loadTransformationMatrix(glm::mat4 matrix)
{ 
	this->loadMat4(this->location_transformationMatrix, matrix); 
}

void TextShader::loadProjectionMatrix(glm::mat4 matrix)
{
	this->loadMat4(this->location_projectionMatrix, matrix);
}

void TextShader::loadViewMatrix(glm::mat4 matrix)
{
	this->loadMat4(this->location_viewMatrix, matrix);
}

void TextShader::loadTextColor(glm::vec3 color)
{
	this->loadVec3(this->location_textColor, color);
}
