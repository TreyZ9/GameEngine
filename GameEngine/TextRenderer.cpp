#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#include "OpenGLFunctions.h"
#include "Debug.h"

TextRenderer::TextRenderer()
{
	// init freetype
	FT_Library ft;
	LOG_freetypeLibraryLoad(!FT_Init_FreeType(&ft));
	FT_Face face;
	LOG_freetypeFontLoad(!FT_New_Face(ft, "C:\\Windows\\fonts\\arial.ttf", 0, &face), "font:arial");

	// load character textures
	glCall(glPixelStorei, GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LOG_freetypeCharacterLoad(false, "");
			continue;
		}

		FT_Glyph glyph;
		FT_Get_Glyph(face->glyph, &glyph);

		if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
			FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph glyphBitmap = (FT_BitmapGlyph)glyph;

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
}

TextRenderer::~TextRenderer() {}

Character TextRenderer::getCharacter(char character)
{
	return this->characters[character];
}