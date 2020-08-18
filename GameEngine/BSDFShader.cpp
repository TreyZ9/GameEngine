
#include "BSDFShader.h"

#include "Camera.h"
#include "Maths.h"

#include <iostream>

BSDFShader::BSDFShader()
{
	this->location_transformationMatrix = NULL;
	this->location_projectionMatrix = NULL;
	this->location_viewMatrix = NULL;
	this->location_materialKa = NULL;
	this->location_materialKd = NULL;
	this->location_materialKs = NULL;
	this->location_materialKe = NULL;
	this->location_materialNi = NULL;
	this->location_materialD = NULL;
	this->location_materialIllum = NULL;
}

BSDFShader::BSDFShader(std::string vertexShaderFilename, std::string fragmentShaderFilename) : ShaderProgram::ShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	this->getAllUniformLocations();
}

BSDFShader::~BSDFShader() { }

void BSDFShader::bindAttributes()
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
	this->bindAttribute(3, "tangent");
	this->bindAttribute(4, "bitangent");
}

void BSDFShader::getAllUniformLocations()
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
}

void BSDFShader::loadTransformationMatrix(glm::mat4 matrix)
{
	this->loadMat4(this->location_transformationMatrix, matrix);
}

void BSDFShader::loadProjectionMatrix(glm::mat4 matrix)
{
	this->loadMat4(this->location_projectionMatrix, matrix);
}

void BSDFShader::loadViewMatrix()
{
	this->loadMat4(this->location_viewMatrix, Camera::viewMatrix);
}

void BSDFShader::loadMaterialInfo(Material mat)
{
	this->loadVec3(this->location_materialKa, mat.Ka);
	this->loadVec3(this->location_materialKd, mat.Kd);
	this->loadVec3(this->location_materialKs, mat.Ks);
	this->loadVec3(this->location_materialKe, mat.Ke);
	this->loadFloat(this->location_materialNi, mat.Ni);
	this->loadFloat(this->location_materialD, mat.d);
	this->loadInt(this->location_materialIllum, mat.illum);
}
