#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include <string>

class ShaderProgram 
{
protected:
	int programID;
	int vertexShaderID;
	int fragmentShaderID;
	int tessellationControlShaderID;
	int tessellationEvaluationShaderID;
	int geometryShaderID;

	int getUniformLocation(std::string uniformName);

	virtual void getAllUniformLocations() = 0 {};

	void loadBoolean(int location, bool value);

	void loadInt(int location, int value);

	void loadFloat(int location, float value);

	void loadVec3(int location, glm::vec3 vector);

	void loadMat4(int location, const glm::mat4 &value);


public:
	ShaderProgram();

	ShaderProgram(std::string vertexFilename, std::string fragmentFilename, std::string tessellationControlFilename = "null", std::string tessellationEvaluationFilename = "null", std::string geometryFilename = "null");

	~ShaderProgram();

	void cleanUp();

	void start();

	void stop();

	void bindAttribute(int attribute, std::string variableName);

	virtual void bindAttributes() = 0 {};

	int loadShader(std::string filename, int type);

	int getProgramID();
};
