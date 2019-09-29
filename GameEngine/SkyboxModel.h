#pragma once

#include "SkyboxShader.h"
#include "Texture.h"
#include "Model.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <vector>

struct SkyboxModel
{
private:
	float size = 500.0f;
	std::vector<float> vertices = {
		-size, size, -size,
		-size, -size, -size,
		size, -size, -size,
		size, -size, -size,
		size, size, -size,
		-size, size, -size,

		-size, -size, size,
		-size, -size, -size,
		-size, size, -size,
		-size, size, -size,
		-size, size, size,
		-size, -size, size,

		size, -size, -size,
		size, -size, size,
		size, size, size,
		size, size, size,
		size, size, -size,
		size, -size, -size,

		-size, -size, size,
		-size, size, size,
		size, size, size,
		size, size, size,
		size, -size, size,
		-size, -size, size,

		-size, size, -size,
		size, size, -size,
		size, size, size,
		size, size, size,
		-size, size, size,
		-size, size, -size,

		-size, -size, -size,
		-size, -size, size,
		size, -size, -size,
		size, -size, -size,
		-size, -size, size,
		size, -size, size
	};
	GLuint vao;
	GLuint vbo;
	Texture texture;

public:
	SkyboxModel(const std::string& directory);

	~SkyboxModel();

	void draw(SkyboxShader shader);
};
