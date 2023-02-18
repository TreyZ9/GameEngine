#pragma once

#include "DisplayManager.h"
#include "TextShader.h"

#include <glad/glad.h>

#include <glm/common.hpp>

#include <string>
#include <vector>
#include <array>
#include <map>


enum class Align {
	left, center, right
};

enum class Origin {
	topLeft, top, topRight,
	left, center, right,
	bottomLeft, bottom, bottomRight
};

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
	float lineSpacing = 1.0f;

	unsigned int textureWidth = 0;
	unsigned int textureHeight = 0;
	GLuint textureID = 0;

public:
	Font() = default;
	Font(const std::string& fontName, const unsigned int fontQuality);
	~Font() = default;

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
	void drawText(const std::string& text, const glm::vec2& pos, const glm::vec2& scale, const Align alignment, const Origin origin);

public:
	TextRenderer();
	~TextRenderer() = default;

	void drawText(const Display& display, TextShader shader, const std::string& text, const glm::vec3& pos, const glm::vec3& rot,
		const glm::vec2& scale = glm::vec2(24.0f), const glm::vec3& color = glm::vec3(1.0f), const Align = Align::center, const Origin origin = Origin::center);
	void drawTextOnHUD(const Display& display, TextShader shader, const std::string& text, const glm::vec2& pos, const glm::vec2& scale = glm::vec2(24.0f),
		const glm::vec3& color = glm::vec3(1.0f), const Align alignment = Align::center, const Origin origin = Origin::center);
};