#pragma once

#include <glad/glad.h>

#include <glm/common.hpp>

#include <map>

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

public:
	TextRenderer();
	~TextRenderer();
};