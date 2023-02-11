#pragma once

#include <glad/glad.h>

#include <glm/common.hpp>

#include <vector>
#include <string>
#include <map>

#include "TextShader.h"

struct Character
{
	float textureAtlasOffset;
	glm::ivec2 size;
	glm::ivec2 bearing;
	glm::ivec2 advance;
};

struct TextureAtlas
{
	GLuint textureID;
	unsigned int width;
	unsigned int height;
};

class TextRenderer
{
private:
	std::map<char, Character> characters;
	GLuint vao;
	GLuint vbo;
	TextureAtlas textureAtlas;

	void loadFont(const std::string& font);
	std::vector<std::string> splitString(const std::string& text, const std::string& delimiter);

public:
	TextRenderer();
	~TextRenderer();

	void drawText(TextShader shader, const std::string& text, glm::vec3 pos, glm::vec3 rot, glm::vec2 scale, glm::vec3 color = glm::vec3(1.0f));
	void drawTextOnHUD(TextShader shader, const std::string& text, glm::vec2 pos, glm::vec2 scale = glm::vec2(1.0f), glm::vec3 color = glm::vec3(1.0f), std::string alignment = "left", std::string origin = "center");
};