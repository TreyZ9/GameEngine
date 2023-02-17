#pragma once

#include "ShaderProgram.h"
#include "Light.h"

#include <glm/gtc/matrix_transform.hpp>

class NormalShader : public ShaderProgram 
{
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
	int location_cameraPosition;

	NormalShader() = default;

	NormalShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	~NormalShader() = default;

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(const glm::mat4& matrix);

	void loadProjectionMatrix(const glm::mat4& matrix);

	void loadViewMatrix();

	void loadLight(const Light& light);

	void loadGamma(const float gamma);

	void loadCameraPosition(const glm::vec3& cameraPositon);
};
