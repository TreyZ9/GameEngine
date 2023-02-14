#pragma once

#include <glad/glad.h>

#include <glm/common.hpp>

#include <vector>
#include <string>
#include <array>
#include <map>

#include "TextShader.h"

struct Character
{
	float textureAtlasOffset;
	glm::ivec2 textureSize;
	glm::vec2 size;
	glm::vec2 bearing;
	glm::vec2 advance;
};

class Font
{
private:

public:
	std::map<char, Character> characters;
	unsigned int textureWidth;
	unsigned int textureHeight;
	float lineHeight;
	GLuint textureID;

	Font();
	Font(const std::string& fontName, unsigned int fontQuality);
	~Font();

	std::array<std::array<float, 4>, 6> generateVertices(const char c, glm::vec2& cursorPos, const glm::vec2& scale);
};

class TextRenderer
{
private:
	glm::vec2 cursorPos;
	GLuint vao;
	GLuint vbo;
	Font font;

	std::vector<std::string> splitString(const std::string& text, const std::string& delimiter);
	void drawText(const std::string& text, glm::vec2 pos, glm::vec2 scale, std::string alignment, std::string origin);

public:
	TextRenderer();
	~TextRenderer();

	void drawText(TextShader shader, const std::string& text, glm::vec3 pos, glm::vec3 rot, glm::vec2 scale, glm::vec3 color = glm::vec3(1.0f), std::string alignment = "left", std::string origin = "center");
	void drawTextOnHUD(TextShader shader, const std::string& text, glm::vec2 pos, glm::vec2 scale = glm::vec2(1.0f), glm::vec3 color = glm::vec3(1.0f), std::string alignment = "left", std::string origin = "center");
};