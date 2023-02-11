#pragma once

#include <glad/glad.h>

#include <glm/common.hpp>

#include <string>
#include <map>

#include "TextShader.h"

struct Character
{
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class TextRenderer
{
private:
	std::map<char, Character> characters;
	GLuint vao;
	GLuint vbo;

public:
	TextRenderer();
	~TextRenderer();

	void drawText(TextShader shader, const std::string& text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
};