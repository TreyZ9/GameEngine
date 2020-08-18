#pragma once

#include "ShaderProgram.h"
#include "Light.h"

class StaticShader: public ShaderProgram {
private:
	std::string VERTEX_SHADER_FILENAME = "vertexShader.vert";
	std::string FRAGMENT_SHADER_FILENAME = "fragmentShader.frag";
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_lightPosition;
	int location_lightColor;
	int location_gamma;

	StaticShader();

	StaticShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	~StaticShader();

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(glm::mat4 matrix);

	void loadProjectionMatrix(glm::mat4);

	void loadViewMatrix();

	void loadLight(Light light);

	void loadGamma(float gamma);
};

