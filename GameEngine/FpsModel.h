#pragma once

#include "FPSShader.h"
#include "Config.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>
#include <vector>
#include <map>

class FpsModel
{
private:
	std::vector<float> vertices = {
		-0.5f,  1.0f, 0.0f,
		-0.5f, -1.0f, 0.0f,
		0.5f, -1.0f, 0.0f,
		0.5f,  1.0f, 0.0f
	};
	std::vector<GLuint> indices = {
		0, 1, 3,
		3, 1, 2
	};
	std::vector<float> textureCoords = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	std::vector<int> fpsBuffer = std::vector<int>(Config::Display::FPS_BUFFER_SIZE, 60);
	std::string fps = "60";

	int vaoID;
	int vertexCount;

	std::map<char, GLuint> fpsTextures;

public:
	FpsModel();
	~FpsModel();

	GLuint getTexture(char number);

	void update();

	void render(FPSShader shader);
};
