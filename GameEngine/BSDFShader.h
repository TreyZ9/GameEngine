#pragma once

#include "ShaderProgram.h"

#include "Material.h"

class BSDFShader : public ShaderProgram {
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

	BSDFShader();

	BSDFShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	~BSDFShader();

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(glm::mat4 matrix);

	void loadProjectionMatrix(glm::mat4);

	void loadViewMatrix();

	void loadMaterialInfo(Material mat);
};
