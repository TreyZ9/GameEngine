#pragma once

#include "ShaderProgram.h"

#include "Material.h"

class BSDFShader : public ShaderProgram 
{
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_materialKa;
	int location_materialKd;
	int location_materialKs;
	int location_materialKe;
	int location_materialNi;
	int location_materialD;
	int location_materialIllum;

	BSDFShader() = default;

	BSDFShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	~BSDFShader() = default;

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(const glm::mat4& matrix);

	void loadProjectionMatrix(const glm::mat4 matrix);

	void loadViewMatrix();

	void loadMaterialInfo(const Material& mat);
};
