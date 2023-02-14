#pragma once

#include <glad/glad.h>

#include <glm/common.hpp>

#include <string>
#include <vector>
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
	std::map<char, Character> characters;
	float lineSpacing;

	unsigned int textureWidth;
	unsigned int textureHeight;
	GLuint textureID;

public:
	Font();
	Font(const std::string& fontName, unsigned int fontQuality);
	~Font();

	std::array<std::array<float, 4>, 6> generateVertices(const char c, glm::vec2& cursorPos, const glm::vec2& scale);
	GLuint getTextureID();
	float calculateLineWidth(const std::string& text, const glm::vec2& scale);
	float getLineHeight(const glm::vec2 scale);
};

class TextRenderer
{
private:
	glm::vec2 cursorPos;
	GLuint vao;
	GLuint vbo;
	Font font;

	std::vector<std::string> splitString(const std::string& text, const std::string& delimiter);
	void drawText(const std::string& text, const glm::vec2& pos, const glm::vec2& scale, std::string alignment, std::string origin);

public:
	TextRenderer();
	~TextRenderer();

	void drawText(TextShader shader, const std::string& text, const glm::vec3& pos, const glm::vec3& rot,
		const glm::vec2& scale = glm::vec2(24.0f), const glm::vec3& color = glm::vec3(1.0f), const std::string& alignment = "center", const std::string& origin = "center");
	void drawTextOnHUD(TextShader shader, const std::string& text, const glm::vec2& pos, const glm::vec2& scale = glm::vec2(24.0f),
		const glm::vec3& color = glm::vec3(1.0f), const std::string& alignment = "center", const std::string& origin = "center");
};