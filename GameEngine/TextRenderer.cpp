#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#include <algorithm>
#include <vector>

#include "OpenGLFunctions.h"
#include "TextShader.h"
#include "Config.h"
#include "Camera.h"
#include "Debug.h"
#include "Maths.h"

TextRenderer::TextRenderer()
{
	this->loadFont("arial");

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

TextRenderer::~TextRenderer() {}

void TextRenderer::loadFont(const std::string& font)
{
	// init freetype
	FT_Library ft;
	LOG_freetypeLibraryLoad(!FT_Init_FreeType(&ft));
	FT_Face face;
	LOG_freetypeFontLoad(!FT_New_Face(ft, ("C:\\Windows\\fonts\\" + font + ".ttf").c_str(), 0, &face), "font:" + font);

	// cache bitmaps and advance to populate texture atlas
	std::vector<FT_BitmapGlyph> bitmaps;
	std::vector<glm::ivec2> advance;
	unsigned int width = 0;
	unsigned int height = 0;
	for (unsigned char c = 0; c < 128; c++)
	{
		// load char
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LOG_freetypeCharacterLoad(false, "");
			continue;
		}

		// render bitmap
		FT_Glyph glyph;
		FT_Get_Glyph(face->glyph, &glyph);
		if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
			FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph glyphBitmap = (FT_BitmapGlyph)glyph;

		bitmaps.push_back(glyphBitmap);
		advance.push_back(glm::ivec2(face->glyph->advance.x, face->glyph->advance.y));
		width += glyphBitmap->bitmap.width + 2; // add 2px buffer to help with aa bleed
		height = std::max(height, glyphBitmap->bitmap.rows);
	}

	// create empty texture atlas
	GLuint textureID;
	glCall(glGenTextures, 1, &textureID);
	glCall(glBindTexture, GL_TEXTURE_2D, textureID);
	glCall(glPixelStorei, GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->textureAtlas = {
		textureID,
		width,
		height
	};

	// populate atlas with textures and create character map
	int x = 0;
	for (unsigned int i = 0; i < 128; i++)
	{
		glCall(glTexSubImage2D, GL_TEXTURE_2D, 0, x, 0, bitmaps[i]->bitmap.width, bitmaps[i]->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, bitmaps[i]->bitmap.buffer);

		Character character = {
			x,
			glm::ivec2(bitmaps[i]->bitmap.width, bitmaps[i]->bitmap.rows),
			glm::ivec2(bitmaps[i]->left, bitmaps[i]->top),
			advance[i]
		};
		this->characters.insert(std::pair<char, Character>((char)i, character));
		x += bitmaps[i]->bitmap.width + 2; // 2px buffer
	}
}

void TextRenderer::drawText(TextShader shader, const std::string& text, glm::vec3 pos, glm::vec3 rot, glm::vec2 scale, glm::vec3 color)
{
	shader.start();

	glm::mat4 projectionMatrix = glm::perspective(Config::Display::FOV, (float)Config::Display::WIDTH /
		(float)Config::Display::HEIGHT, Config::Display::NEAR_PLANE, Config::Display::FAR_PLANE);

	glm::mat4 transformationMatrix;
	Maths::createTransformationMatrix(transformationMatrix, pos, rot.x, rot.y, rot.z, 1.0f);

	shader.loadTransformationMatrix(transformationMatrix);
	shader.loadProjectionMatrix(projectionMatrix);
	shader.loadViewMatrix(Camera::viewMatrix);
	shader.loadTextColor(color);

	glCall(glActiveTexture, GL_TEXTURE0);
	glCall(glBindVertexArray, this->vao);
	glCall(glBindTexture, GL_TEXTURE_2D, this->textureAtlas.textureID);

	for (char c : text)
	{
		Character character = this->characters[c];

		float xpos = pos.x + character.bearing.x * scale.x;
		float ypos = pos.y - (character.size.y - character.bearing.y) * scale.y;

		float w = character.size.x * scale.x;
		float h = character.size.y * scale.y;

		float x0 = character.textureAtlasOffset / this->textureAtlas.width;
		float x1 = (character.textureAtlasOffset + character.size.x) / this->textureAtlas.width;
		
		float y1 = (float)character.size.y / (float)this->textureAtlas.height;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   x0, 0.0f },
			{ xpos,     ypos,       x0, y1 },
			{ xpos + w, ypos,       x1, y1 },

			{ xpos,     ypos + h,   x0, 0.0f },
			{ xpos + w, ypos,       x1, y1 },
			{ xpos + w, ypos + h,   x1, 0.0f }
		};

		glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
		glCall(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		pos.x += (character.advance.x >> 6) * scale.x;
	}

	glCall(glBindVertexArray, 0);
	glCall(glBindTexture, GL_TEXTURE_2D, 0);

	shader.stop();
}

void TextRenderer::drawTextOnHUD(TextShader shader, const std::string& text, glm::vec2 pos, glm::vec2 scale, glm::vec3 color)
{
	shader.start();

	glm::mat4 projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	shader.loadTransformationMatrix(glm::mat4(1.0f));
	shader.loadProjectionMatrix(projectionMatrix);
	shader.loadViewMatrix(glm::mat4(1.0f));
	shader.loadTextColor(color);

	glCall(glActiveTexture, GL_TEXTURE0);
	glCall(glBindVertexArray, this->vao);
	glCall(glBindTexture, GL_TEXTURE_2D, this->textureAtlas.textureID);

	for (char c : text)
	{
		Character character = this->characters[c];

		float xpos = pos.x + character.bearing.x * scale.x;
		float ypos = pos.y - (character.size.y - character.bearing.y) * scale.y;

		float w = character.size.x * scale.x;
		float h = character.size.y * scale.y;

		float x0 = character.textureAtlasOffset / this->textureAtlas.width;
		float x1 = (character.textureAtlasOffset + character.size.x) / this->textureAtlas.width;

		float y1 = (float)character.size.y / (float)this->textureAtlas.height;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   x0, 0.0f },
			{ xpos,     ypos,       x0, y1 },
			{ xpos + w, ypos,       x1, y1 },

			{ xpos,     ypos + h,   x0, 0.0f },
			{ xpos + w, ypos,       x1, y1 },
			{ xpos + w, ypos + h,   x1, 0.0f }
		};

		glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
		glCall(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		pos.x += (character.advance.x >> 6) * scale.x;
	}

	glCall(glBindVertexArray, 0);
	glCall(glBindTexture, GL_TEXTURE_2D, 0);

	shader.stop();
}