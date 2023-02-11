
#include "TextShader.h"

TextShader::TextShader() {}

TextShader::TextShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

TextShader::~TextShader() {}

void TextShader::bindAttributes()
{
	this->bindAttribute(0, "vertex");
}

void TextShader::getAllUniformLocations()
{
	this->location_transformationMatrix = this->getUniformLocation("projectionMatrix");
	this->location_textColor = this->getUniformLocation("textColor");
}

void TextShader::loadTransformationMatrix(glm::mat4 matrix)
{ 
	this->loadMat4(this->location_transformationMatrix, matrix); 
}

void TextShader::loadTextColor(glm::vec3 color)
{
	this->loadVec3(this->location_textColor, color);
}
