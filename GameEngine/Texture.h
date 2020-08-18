#pragma once

#include <glad/glad.h>

#include <string>

struct Texture
{
	GLuint ID;
	std::string Type;
	std::string Path;
};
