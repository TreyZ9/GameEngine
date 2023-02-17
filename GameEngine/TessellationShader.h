#pragma once

#include "ShaderProgram.h"
#include "Light.h"

class TessellationShader : public ShaderProgram 
{
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_eyePos;
	int location_lightPosition;
	int location_lightColor;
	int location_gamma;
	int location_blackPoint;


	TessellationShader() = default;

	TessellationShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename,
		const std::string& tessellationControlFilename, const std::string& tessellationEvaluationFilename,
		const std::string& geometryFilename = "null");

	~TessellationShader() = default;

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(const glm::mat4& matrix);

	void loadProjectionMatrix(const glm::mat4& matrix);

	void loadViewMatrix();

	void loadCameraPosition(const glm::vec3& pos);

	void loadLight(const Light& light);

	void loadGamma(const float gamma);

	void loadBlackPoint(const float blackPoint);
};
