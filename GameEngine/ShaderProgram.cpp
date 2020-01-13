
#include "ShaderProgram.h"

#include "Debug.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram() {}

ShaderProgram::ShaderProgram(std::string vertexFilename, std::string fragmentFilename, std::string tessellationControlFilename, std::string tessellationEvaluationFilename, std::string geometryFilename)
{
	this->vertexShaderID = this->loadShader(vertexFilename, GL_VERTEX_SHADER);
	this->fragmentShaderID = this->loadShader(fragmentFilename, GL_FRAGMENT_SHADER);

	if (tessellationControlFilename != "null")
		this->tessellationControlShaderID = this->loadShader(tessellationControlFilename, GL_TESS_CONTROL_SHADER);

	if (tessellationEvaluationFilename != "null")
		this->tessellationEvaluationShaderID = this->loadShader(tessellationEvaluationFilename, GL_TESS_EVALUATION_SHADER);

	if (geometryFilename != "null")
		this->geometryShaderID = this->loadShader(geometryFilename, GL_GEOMETRY_SHADER);

	this->programID = glCreateProgram();
	glAttachShader(this->programID, this->vertexShaderID);

	if (tessellationEvaluationFilename != "null")
		glAttachShader(this->programID, this->tessellationEvaluationShaderID);

	if (tessellationControlFilename != "null")
		glAttachShader(this->programID, this->tessellationControlShaderID);

	if (geometryFilename != "null")
		glAttachShader(this->programID, this->geometryShaderID);

	glAttachShader(this->programID, this->fragmentShaderID);
	this->bindAttributes();
	glLinkProgram(this->programID);

	GLint success;
	GLchar infoLog[1024];

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 1024, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	this->getAllUniformLocations();
}

ShaderProgram::~ShaderProgram() {}

int ShaderProgram::getUniformLocation(std::string uniformName) { return glGetUniformLocation(this->programID, uniformName.c_str()); }

void ShaderProgram::loadFloat(int location, float value) { glUniform1f(location, value); }

void ShaderProgram::loadInt(int location, int value) { glUniform1i(location, value); }

void ShaderProgram::loadVec3(int location, glm::vec3 vector) { glUniform3f(location, vector.x, vector.y, vector.z); }

void ShaderProgram::loadBoolean(int location, bool value) {
	if (value)
		glUniform1f(location, 1);
	else
		glUniform1f(location, 0);
}

void ShaderProgram::loadMat4(int location, const glm::mat4& matrix) const { glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]); }

void ShaderProgram::cleanUp() {
	glDetachShader(this->programID, this->vertexShaderID);
	glDetachShader(this->programID, this->fragmentShaderID);
	glDeleteShader(this->vertexShaderID);
	glDeleteShader(this->fragmentShaderID);
	glDeleteProgram(this->programID);
}

void ShaderProgram::start() { glUseProgram(this->programID); }

void ShaderProgram::stop() { glUseProgram(0); }

void ShaderProgram::bindAttribute(int attribute, std::string variableName) { glBindAttribLocation(this->programID, attribute, variableName.c_str()); }

int ShaderProgram::loadShader(std::string filename, int type) {
	std::string data;
	std::ifstream dataFile;
	dataFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		dataFile.open(filename);
		std::stringstream dataStream;
		dataStream << dataFile.rdbuf();
		dataFile.close();
		data = dataStream.str();
	}
	catch (std::ifstream::failure e) { std::cout << "failed to open : " << filename << std::endl; }

	const char* shaderCode = data.c_str();
	unsigned int shader;
	GLint success;
	GLchar infoLog[1024];
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		if (type == GL_VERTEX_SHADER)
			Debug::fileLoad(filename, "vertexShader", false);
		else if (type == GL_FRAGMENT_SHADER)
			Debug::fileLoad(filename, "fragmentShader", false);
		else if (type == GL_TESS_CONTROL_SHADER)
			Debug::fileLoad(filename, "controlShader", false);
		else if (type == GL_TESS_EVALUATION_SHADER)
			Debug::fileLoad(filename, "evaluationShader", false);
		else if (type == GL_GEOMETRY_SHADER)
			Debug::fileLoad(filename, "geometryShader", false);
		else
			Debug::fileLoad(filename, "unknownShader", false);

		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}
	Debug::fileLoad(filename, "shader", true);

	return shader;
}

int ShaderProgram::getProgramID() { return this->programID; }
