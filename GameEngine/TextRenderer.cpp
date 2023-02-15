#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <format>

#include "OpenGLFunctions.h"
#include "DisplayManager.h"
#include "TextShader.h"
#include "Camera.h"
#include "Maths.h"

Font::Font(const std::string& fontName, unsigned int fontQuality = 24)
{
	// load library
	FT_Library library;
	if (FT_Init_FreeType(&library))
	{
		spdlog::error("Could not init FreeType library");
		return;
	}
	spdlog::debug("Loaded FreeType library");

	// load font
	FT_Face face;
	FT_Error error = FT_New_Face(library, std::format("C:\\Windows\\fonts\\{}.ttf", fontName).c_str(), 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		spdlog::error("Could not load font '{}' because format is not supported", fontName);
		return;
	}
	else if (error)
	{
		spdlog::error("Could not load font '{}' because it (does not exist / cannot be opened / is broken)", fontName);
		return;
	}
	spdlog::debug("Loaded font '{}'", fontName);

	// configure font
	if (FT_Set_Char_Size(face, 0, fontQuality << 6, 0, 0))
	{
		spdlog::error("Could not configure font '{}'", fontName);
		return;
	}
	spdlog::debug("Configured font '{}'", fontName);

	// render bitmaps
	std::vector<FT_BitmapGlyph> bitmapGlyphs;
	this->textureWidth = 0;
	this->textureHeight = 0;
	for (unsigned char i = 0; i < 128; i++)
	{
		// load character
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			spdlog::error("Failed to load character '{}' for font '{}'", (char)i, fontName);
			return;
		}

		// load glyph
		FT_Glyph glyph;
		if (FT_Get_Glyph(face->glyph, &glyph))
		{
			spdlog::error("Failed to load glyph '{}' for font '{}'", (char)i, fontName);
			return;
		}

		// render bitmap if necessary
		if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1))
			{
				spdlog::error("Failed to render bitmap '{}' for font '{}'", (char)i, fontName);
				return;
			}
		}

		// cache bitmap
		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
		bitmapGlyphs.push_back((FT_BitmapGlyph)bitmapGlyph);

		// cache character
		Character character = {
			this->textureWidth,
			glm::ivec2(bitmapGlyph->bitmap.width, bitmapGlyph->bitmap.rows),
			glm::vec2((float)bitmapGlyph->bitmap.width / fontQuality, (float)bitmapGlyph->bitmap.rows / fontQuality),
			glm::vec2((float)bitmapGlyph->left / fontQuality, (float)bitmapGlyph->top / fontQuality),
			glm::vec2((float)(face->glyph->advance.x >> 6) / fontQuality, (float)(face->glyph->advance.y >> 6) / fontQuality)
		};
		this->characters.insert(std::pair<char, Character>(i, character));

		// update texture size
		this->textureWidth += bitmapGlyph->bitmap.width + 1;
		this->textureHeight = std::max(this->textureHeight, bitmapGlyph->bitmap.rows);
	}

	// create empty texture atlas
	glCall(glGenTextures, 1, &this->textureID);
	glCall(glBindTexture, GL_TEXTURE_2D, this->textureID);
	glCall(glPixelStorei, GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->textureWidth, this->textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// fill texture atlas
	for (unsigned char i = 0; i < bitmapGlyphs.size(); i++)
	{
		glCall(glTexSubImage2D, GL_TEXTURE_2D, 0, this->characters[i].textureAtlasOffset, 0, bitmapGlyphs[i]->bitmap.width, bitmapGlyphs[i]->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, bitmapGlyphs[i]->bitmap.buffer);
	}
}

std::array<std::array<float, 4>, 6> Font::generateVertices(const char c, glm::vec2& cursorPos, const glm::vec2& scale)
{
	Character character = this->characters.at(c);

	// calculate vertex coords
	glm::vec2 pos = glm::vec2(
		cursorPos.x + character.bearing.x * scale.x,
		cursorPos.y - (character.size.y - character.bearing.y) * scale.y);
	glm::vec2 size = character.size * scale;

	// calculate texture coords
	float s0 = character.textureAtlasOffset / this->textureWidth;
	float s1 = (character.textureAtlasOffset + character.textureSize.x) / this->textureWidth;
	float t1 = (float)character.textureSize.y / this->textureHeight;

	// create vertices
	std::array<std::array<float, 4>, 6> vertices = { {
		{ pos.x,          pos.y + size.y, s0, 0.0f },
		{ pos.x,          pos.y,          s0, t1 },
		{ pos.x + size.x, pos.y,          s1, t1 },

		{ pos.x,          pos.y + size.y, s0, 0.0f },
		{ pos.x + size.x, pos.y,          s1, t1 },
		{ pos.x + size.x, pos.y + size.y, s1, 0.0f }
	} };

	cursorPos.x += character.advance.x * scale.x;
	return vertices;
}

GLuint Font::getTextureID()
{
	return this->textureID;
}

float Font::calculateLineWidth(const std::string& text, const glm::vec2& scale)
{
	float lineWidth = 0.0f;
	for (char c : text)
	{
		lineWidth += this->characters.at(c).advance.x * scale.x;
	}
	return lineWidth;
}

float Font::getLineHeight(const glm::vec2 scale)
{
	return this->lineSpacing * scale.y;
}

TextRenderer::TextRenderer()
{
	this->font = Font("arial");
	this->cursorPos = glm::vec2(0.0f);

	// create render object
	glCall(glGenVertexArrays, 1, &this->vao);
	glCall(glGenBuffers, 1, &this->vbo);
	glCall(glBindVertexArray, this->vao);
	glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glCall(glEnableVertexAttribArray, 0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
	glCall(glBindVertexArray, 0);
}

std::vector<std::string> TextRenderer::splitString(const std::string& text, const std::string& delimiter)
{
	std::vector<std::string> elements;
	size_t last = 0;
	size_t next = 0;
	while ((next = text.find(delimiter, last)) != std::string::npos)
	{
		elements.push_back(text.substr(last, next - last));
		last = next + 1;
	}
	elements.push_back(text.substr(last));
	return elements;
}

void TextRenderer::drawText(const std::string& text, const glm::vec2& pos, const glm::vec2& scale, std::string alignment, std::string origin)
{
	// bind vao and texture atlas
	glCall(glActiveTexture, GL_TEXTURE0);
	glCall(glBindVertexArray, this->vao);
	glCall(glBindTexture, GL_TEXTURE_2D, this->font.getTextureID());

	// calculate text bounding box
	glm::vec2 boundingBox(0.0f);

	std::vector<std::string> lines = this->splitString(text, "\n");
	std::vector<float> lineOffsets;

	for (std::string line : lines)
	{
		float lineWidth = this->font.calculateLineWidth(line, scale);
		lineOffsets.push_back(lineWidth);
		boundingBox.x = std::max(boundingBox.x, lineWidth);
	}
	boundingBox.y = this->font.getLineHeight(scale) * lines.size();

	// alignment
	for (float& offset : lineOffsets)
	{
		if (alignment == "left")
		{
			offset = 0.0f;
		}
		else if (alignment == "center")
		{
			offset = (boundingBox.x - offset) / 2.0f;
		}
		else
		{
			offset = (boundingBox.x - offset);
		}
	}

	// origin horizontal
	for (float& offset : lineOffsets)
	{
		if (origin == "top" || origin == "center" || origin == "bottom") // center column
		{
			offset -= boundingBox.x / 2.0f;
		}
		else if (origin == "topright" || origin == "right" || origin == "bottomright") // right column
		{
			offset -= boundingBox.x;
		}
	}

	// origin vertical
	float yOffset = boundingBox.y - this->font.getLineHeight(scale);
	if (origin == "left" || origin == "center" || origin == "right") // center row
	{
		yOffset -= boundingBox.y / 2.0f;
	}
	else if (origin == "topleft" || origin == "bottom" || origin == "topright") // top row
	{
		yOffset -= boundingBox.y;
	}

	// draw each character
	this->cursorPos.y = pos.y + yOffset;
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		this->cursorPos.x = pos.x + lineOffsets[i];
		for (char& c : lines[i])
		{
			std::array<std::array<float, 4>, 6> vertices = this->font.generateVertices(c, cursorPos, scale);

			glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
			glCall(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
			glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		this->cursorPos.y -= this->font.getLineHeight(scale);
	}

	// unbind
	glCall(glBindVertexArray, 0);
	glCall(glBindTexture, GL_TEXTURE_2D, 0);
}

void TextRenderer::drawText(TextShader shader, const std::string& text, const glm::vec3& pos, const glm::vec3& rot,
	const glm::vec2& scale, const glm::vec3& color, const std::string& alignment, const std::string& origin)
{
	shader.start();

	glm::mat4 transformationMatrix;
	Maths::createTransformationMatrix(transformationMatrix, pos, rot.x, rot.y, rot.z, 1.0f);

	shader.loadTransformationMatrix(transformationMatrix);
	shader.loadProjectionMatrix(DisplayManager::getProjectionMatrix());
	shader.loadViewMatrix(Camera::viewMatrix);
	shader.loadTextColor(color);

	this->drawText(text, glm::vec2(0.0f), scale, alignment, origin);

	shader.stop();
}

void TextRenderer::drawTextOnHUD(TextShader shader, const std::string& text, const glm::vec2& pos, const glm::vec2& scale,
	const glm::vec3& color, const std::string& alignment, const std::string& origin)
{
	shader.start();

	glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)DisplayManager::getResolution().x, 0.0f, (float)DisplayManager::getResolution().y);
	shader.loadTransformationMatrix(glm::mat4(1.0f));
	shader.loadProjectionMatrix(projectionMatrix);
	shader.loadViewMatrix(glm::mat4(1.0f));
	shader.loadTextColor(color);

	this->drawText(text, pos, scale, alignment, origin);

	shader.stop();
}