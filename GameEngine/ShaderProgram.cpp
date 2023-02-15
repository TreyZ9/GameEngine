
#include "ShaderProgram.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <spdlog/spdlog.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "OpenGLFunctions.h"

ShaderProgram::ShaderProgram(std::string vertexFilename, std::string fragmentFilename, std::string tessellationControlFilename, std::string tessellationEvaluationFilename, std::string geometryFilename)
{
	this->vertexShaderID = this->loadShader(vertexFilename, GL_VERTEX_SHADER);
	this->fragmentShaderID = this->loadShader(fragmentFilename, GL_FRAGMENT_SHADER);

	if (tessellationControlFilename != "null")
	{
		this->tessellationControlShaderID = this->loadShader(tessellationControlFilename, GL_TESS_CONTROL_SHADER);
	}

	if (tessellationEvaluationFilename != "null")
	{
		this->tessellationEvaluationShaderID = this->loadShader(tessellationEvaluationFilename, GL_TESS_EVALUATION_SHADER);
	}

	if (geometryFilename != "null")
	{
		this->geometryShaderID = this->loadShader(geometryFilename, GL_GEOMETRY_SHADER);
	}

	this->programID = glCall(glCreateProgram);
	glCall(glAttachShader, this->programID, this->vertexShaderID);

	if (tessellationEvaluationFilename != "null")
	{
		glCall(glAttachShader, this->programID, this->tessellationEvaluationShaderID);
	}

	if (tessellationControlFilename != "null")
	{
		glCall(glAttachShader, this->programID, this->tessellationControlShaderID);
	}

	if (geometryFilename != "null")
	{
		glCall(glAttachShader, this->programID, this->geometryShaderID);
	}

	glCall(glAttachShader, this->programID, this->fragmentShaderID);
	this->bindAttributes();
	glCall(glLinkProgram, this->programID);

	GLint success;
	GLchar infoLog[1024];

	glCall(glGetProgramiv, programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 1024, NULL, infoLog); // glCall Currently Incompatible, Needs Fixed
		// spdlog::error("Error loading shader, {}", infoLog);
	}

	this->getAllUniformLocations();
}

int ShaderProgram::getUniformLocation(std::string uniformName) 
{ 
	return glCall(glGetUniformLocation, this->programID, uniformName.c_str()); 
}

void ShaderProgram::loadFloat(int location, float value) 
{ 
	glCall(glUniform1f, location, value); 
}

void ShaderProgram::loadInt(int location, int value) 
{ 
	glCall(glUniform1i, location, value); 
}

void ShaderProgram::loadVec3(int location, glm::vec3 vector) 
{ 
	glCall(glUniform3f, location, vector.x, vector.y, vector.z); 
}

void ShaderProgram::loadBoolean(int location, bool value) 
{
	if (value)
	{
		glCall(glUniform1f, location, 1);
	}
	else
	{
		glCall(glUniform1f, location, 0);
	}
}

void ShaderProgram::loadMat4(int location, const glm::mat4& matrix)
{ 
	glCall(glUniformMatrix4fv, location, 1, GL_FALSE, &matrix[0][0]); 
}

void ShaderProgram::cleanUp() 
{
	glCall(glDetachShader, this->programID, this->vertexShaderID);
	glCall(glDetachShader, this->programID, this->fragmentShaderID);
	glCall(glDeleteShader, this->vertexShaderID);
	glCall(glDeleteShader, this->fragmentShaderID);
	glCall(glDeleteProgram, this->programID);
}

void ShaderProgram::start() 
{ 
	glCall(glUseProgram, this->programID); 
}

void ShaderProgram::stop() 
{ 
	glCall(glUseProgram, 0); 
}

void ShaderProgram::bindAttribute(int attribute, std::string variableName) 
{ 
	glCall(glBindAttribLocation, this->programID, attribute, variableName.c_str()); 
}

int ShaderProgram::loadShader(std::string filename, int type) 
{
	std::string data;
	std::ifstream dataFile;
	dataFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try 
	{
		dataFile.open(filename);
		std::stringstream dataStream;
		dataStream << dataFile.rdbuf();
		dataFile.close();
		data = dataStream.str();
	}
	catch (std::ifstream::failure e) 
	{
		spdlog::error("Could not load shader '{}' because file does not exist", filename);
	}

	const char* shaderCode = data.c_str();
	unsigned int shader;
	GLint success;
	GLchar infoLog[1024];
	shader = glCall(glCreateShader, type);
	glShaderSource(shader, 1, &shaderCode, NULL); // glCall Currently Incompatible, Needs Fixed
	glCall(glCompileShader, shader);
	glCall(glGetShaderiv, shader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		if (type == GL_VERTEX_SHADER)
		{
			spdlog::error("Could not load vertex shader '{}'", filename);
		}
		else if (type == GL_FRAGMENT_SHADER)
		{
			spdlog::error("Could not load fragment shader '{}'", filename);
		}
		else if (type == GL_TESS_CONTROL_SHADER)
		{
			spdlog::error("Could not load tesselation control shader '{}'", filename);
		}
		else if (type == GL_TESS_EVALUATION_SHADER)
		{
			spdlog::error("Could not load tesselation evaluation shader '{}'", filename);
		}
		else if (type == GL_GEOMETRY_SHADER)
		{
			spdlog::error("Could not load geometry shader '{}'", filename);
		}
		else
		{
			spdlog::error("Could not load unknown shader '{}'", filename);
		}

		glGetShaderInfoLog(shader, 1024, NULL, infoLog); // glCall Currently Incompatible, Needs Fixed
		std::cout << infoLog << std::endl;
	}
	spdlog::debug("Loaded shader '{}'", filename);

	return shader;
}

int ShaderProgram::getProgramID() 
{ 
	return this->programID; 
}
