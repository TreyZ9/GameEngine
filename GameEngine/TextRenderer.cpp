#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#include <algorithm>
#include <vector>

#include "OpenGLFunctions.h"
#include "TextShader.h"
#include "Debug.h"

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
		width += glyphBitmap->bitmap.width;
		height = std::max(height, glyphBitmap->bitmap.rows);
	}

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
		x += bitmaps[i]->bitmap.width;
	}
}

void TextRenderer::drawText(TextShader shader, const std::string& text, 
	float x, float y, float scale, glm::vec3 color)
{
	shader.start();

	glm::mat4 projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	shader.loadTransformationMatrix(projectionMatrix);
	shader.loadTextColor(color);

	glCall(glActiveTexture, GL_TEXTURE0);
	glCall(glBindVertexArray, this->vao);
	glCall(glBindTexture, GL_TEXTURE_2D, this->textureAtlas.textureID);

	for (char c : text)
	{
		Character character = this->characters[c];

		float xpos = x + character.bearing.x * scale;
		float ypos = y - (character.size.y - character.bearing.y) * scale;

		float w = character.size.x * scale;
		float h = character.size.y * scale;

		float x0 = character.textureAtlasOffset / this->textureAtlas.width;
		float x1 = (character.textureAtlasOffset + character.size.x) / this->textureAtlas.width;

		float y0 = 0.0f;
		float y1 = (float)character.size.y / (float)this->textureAtlas.height;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   x0, y0 },
			{ xpos,     ypos,       x0, y1 },
			{ xpos + w, ypos,       x1, y1 },

			{ xpos,     ypos + h,   x0, y0 },
			{ xpos + w, ypos,       x1, y1 },
			{ xpos + w, ypos + h,   x1, y0 }
		};

		glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
		glCall(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (character.advance.x >> 6) * scale;
	}

	glCall(glBindVertexArray, 0);
	glCall(glBindTexture, GL_TEXTURE_2D, 0);

	shader.stop();
}