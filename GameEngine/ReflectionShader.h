#pragma once

#include "ShaderProgram.h"
#include "Material.h"
#include "Light.h"

#include <glad/glad.h>

#include <vector>
#include <array>

class ReflectionShader : public ShaderProgram 
{
private:
	const unsigned int MAX_LIGHTS = 4;

	std::string VERTEX_SHADER_FILENAME = "vertexShader.vert";
	std::string FRAGMENT_SHADER_FILENAME = "fragmentShader.frag";
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
	std::array<int, 4> location_lightPositions;
	std::array<int, 4> location_lightColors;
	int location_cameraPosition;

	ReflectionShader() = default;

	ReflectionShader(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	~ReflectionShader() = default;

	void bindAttributes();

	void getAllUniformLocations();

	void loadTransformationMatrix(glm::mat4 matrix);

	void loadProjectionMatrix(glm::mat4);

	void loadViewMatrix();

	void loadMaterialInfo(Material mat);

	void loadLights(std::vector<Light> lights);

	void loadCameraPosition();

	void loadCubemap(const GLuint& textureID);
};
