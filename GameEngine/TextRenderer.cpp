#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#include "OpenGLFunctions.h"
#include "TextShader.h"
#include "Debug.h"

TextRenderer::TextRenderer()
{
	// init freetype
	FT_Library ft;
	LOG_freetypeLibraryLoad(!FT_Init_FreeType(&ft));
	FT_Face face;
	LOG_freetypeFontLoad(!FT_New_Face(ft, "C:\\Windows\\fonts\\arial.ttf", 0, &face), "font:arial");

	// load character textures into map
	glCall(glPixelStorei, GL_UNPACK_ALIGNMENT, 1);
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

		// load texture into buffer and configure
		GLuint texture;
		glCall(glGenTextures, 1, &texture);
		glCall(glBindTexture, GL_TEXTURE_2D, texture);
		glCall(glTexImage2D, GL_TEXTURE_2D, 0, GL_RED, glyphBitmap->bitmap.width,
			glyphBitmap->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, glyphBitmap->bitmap.buffer);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(glyphBitmap->bitmap.width, glyphBitmap->bitmap.rows),
			glm::ivec2(glyphBitmap->left, glyphBitmap->top),
			face->glyph->advance.x
		};

		this->characters.insert(std::pair<char, Character>(c, character));
	}

	// unload freetype
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

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

void TextRenderer::drawText(TextShader shader, const std::string& text, float x, float y, float scale, glm::vec3 color)
{
	shader.start();

	glm::mat4 projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	shader.loadTransformationMatrix(projectionMatrix);
	shader.loadTextColor(color);

	glCall(glActiveTexture, GL_TEXTURE0);
	glCall(glBindVertexArray, this->vao);

	for (char c : text)
	{
		Character character = this->characters[c];;

		float xpos = x + character.bearing.x * scale;
		float ypos = y - (character.size.y - character.bearing.y) * scale;

		float w = character.size.x * scale;
		float h = character.size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glCall(glBindTexture, GL_TEXTURE_2D, character.textureID);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
		glCall(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (character.advance >> 6) * scale;
	}

	glCall(glBindVertexArray, 0);
	glCall(glBindTexture, GL_TEXTURE_2D, 0);

	shader.stop();
}