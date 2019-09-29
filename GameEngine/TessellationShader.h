#pragma once

#include "ShaderProgram.h"
#include "Light.h"

class TessellationShader : public ShaderProgram {
public:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_eyePos;
	int location_lightPosition;
	int location_lightColor;
	int location_gamma;
	int location_blackPoint;

	// Default Constructor
	TessellationShader();
	// Constructor
	TessellationShader(std::string vertexShaderFilename, std::string fragmentShaderFilename, 
		std::string tessellationControlFilename, std::string tessellationEvaluationFilename);
	~TessellationShader();
	void bindAttributes();
	void getAllUniformLocations();
	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4);
	void loadViewMatrix();
	void loadCameraPosition(glm::vec3 pos);
	void loadLight(Light light);
	void loadGamma(float gamma);
	void loadBlackPoint(float blackPoint);
};
