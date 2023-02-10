#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

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

		GLuint texture;
		glCall(glGenTextures, 1, &texture);
		glCall(glBindTexture, GL_TEXTURE_2D, texture);
		glCall(glTexImage2D, GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, 
			face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		this->characters.insert(std::pair<char, Character>(c, character));
	}

	// unload freetype
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

TextRenderer::~TextRenderer() {}