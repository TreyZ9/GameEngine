
#include "ReflectionShader.h"

#include "OpenGLFunctions.h"
#include "Camera.h"
#include "Maths.h"

#include <vector>

ReflectionShader::ReflectionShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) : 
	ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

void ReflectionShader::bindAttributes() 
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
	this->bindAttribute(3, "tangent");
	this->bindAttribute(4, "bitangent");
}

void ReflectionShader::getAllUniformLocations() 
{
	this->location_transformationMatrix = this->getUniformLocation("transformationMatrix");
	this->location_projectionMatrix = this->getUniformLocation("projectionMatrix");
	this->location_viewMatrix = this->getUniformLocation("viewMatrix");
	this->location_materialKa = this->getUniformLocation("materialKa");
	this->location_materialKd = this->getUniformLocation("materialKd");
	this->location_materialKs = this->getUniformLocation("materialKs");
	this->location_materialKe = this->getUniformLocation("materialKe");
	this->location_materialNi = this->getUniformLocation("materialNi");
	this->location_materialD = this->getUniformLocation("materialD");
	this->location_materialIllum = this->getUniformLocation("materialIllum");
	this->location_cameraPosition = this->getUniformLocation("cameraPosition");

	for (int i = 0; i < this->MAX_LIGHTS; i++)
	{
		this->location_lightPositions[i] = this->getUniformLocation("lightPositions[" + std::to_string(i) + "]");
		this->location_lightColors[i] = this->getUniformLocation("lightColors[" + std::to_string(i) + "]");
	}
}

void ReflectionShader::loadTransformationMatrix(const glm::mat4& matrix)
{
	this->loadMat4(this->location_transformationMatrix, matrix);
}

void ReflectionShader::loadProjectionMatrix(const glm::mat4& matrix)
{
	this->loadMat4(this->location_projectionMatrix, matrix);
}

void ReflectionShader::loadViewMatrix()
{
	this->loadMat4(this->location_viewMatrix, Camera::viewMatrix);
}

void ReflectionShader::loadMaterialInfo(const Material& mat)
{
	this->loadVec3(this->location_materialKa, mat.Ka);
	this->loadVec3(this->location_materialKd, mat.Kd);
	this->loadVec3(this->location_materialKs, mat.Ks);
	this->loadVec3(this->location_materialKe, mat.Ke);
	this->loadFloat(this->location_materialNi, mat.Ni);
	this->loadFloat(this->location_materialD, mat.d);
	this->loadInt(this->location_materialIllum, mat.illum);
}

void ReflectionShader::loadLights(const std::vector<Light>& lights)
{
	for (int i = 0; i < lights.size() && i < 4; i++)
	{
		this->loadVec3(this->location_lightPositions[i], lights[i].position);
		this->loadVec3(this->location_lightColors[i], lights[i].color);
	}
}

void ReflectionShader::loadCameraPosition()
{
	this->loadVec3(this->location_cameraPosition, Camera::position);
}