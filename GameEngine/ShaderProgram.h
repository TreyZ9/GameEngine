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

	int getUniformLocation(const std::string& uniformName);

	virtual void getAllUniformLocations() = 0 {};

	void loadBoolean(const int location, const bool value);

	void loadInt(const int location, const int value);

	void loadFloat(const int location, const float value);

	void loadVec3(const int location, const glm::vec3& vector);

	void loadMat4(const int location, const glm::mat4 &value);


public:
	ShaderProgram() = default;

	ShaderProgram(const std::string& vertexFilename, const std::string& fragmentFilename, const std::string& tessellationControlFilename = "null", 
		const std::string& tessellationEvaluationFilename = "null", const std::string& geometryFilename = "null");

	~ShaderProgram() = default;

	void cleanUp();

	void start();

	void stop();

	void bindAttribute(const int attribute, const std::string& variableName);

	virtual void bindAttributes() = 0 {};

	int loadShader(const std::string& filename, const int type);

	int getProgramID();
};
