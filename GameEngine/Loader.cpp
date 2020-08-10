#include "Loader.h"

#include "stb_image.h"

#include <fstream>

#include "OpenGLFunctions.h"

unsigned int Loader::createVAO()
{
	unsigned int vaoID;
	glCall(glGenVertexArrays, 1, &vaoID);
	glCall(glBindVertexArray, vaoID);
	return vaoID;
}

unsigned int Loader::bindIndicesArray(std::vector<unsigned int> indices)
{
	unsigned int eboID;
	glCall(glGenBuffers, 1, &eboID);
	glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, eboID);
	glCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	return eboID;
}

void Loader::unbindVAO()
{
	glCall(glBindVertexArray, 0);
}

Texture Loader::loadTexture(const std::string& filename, const std::string& typeName)
{
	unsigned int textureID;
	glCall(glGenTextures, 1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glCall(glBindTexture, GL_TEXTURE_2D, textureID);
		glCall(glTexImage2D, GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glCall(glGenerateMipmap, GL_TEXTURE_2D);

		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename << std::endl;
		std::cout << stbi_failure_reason << std::endl;
		stbi_image_free(data);
	}

	Texture texture;
	texture.ID = textureID;
	texture.Type = typeName;
	texture.Path = filename;

	return texture;
}

Texture Loader::loadCubeMap(const std::string& path)
{
	std::vector<std::string> faces = { path + "/right.png", path + "/left.png", path + "/top.png",
			path + "/bottom.png", path + "/back.png", path + "/front.png" };

	unsigned int textureID;
	glCall(glGenTextures, 1, &textureID);
	glCall(glBindTexture, GL_TEXTURE_CUBE_MAP, textureID);

	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glCall(glTexImage2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			stbi_image_free(data);
		}
	}

	glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	Texture texture;
	texture.ID = textureID;
	texture.Type = "cubeMap";
	texture.Path = path;

	return texture;
}

unsigned int Loader::loadTextureFromPath(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glCall(glGenTextures, 1, &textureID);

	stbi_set_flip_vertically_on_load(false);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glCall(glBindTexture, GL_TEXTURE_2D, textureID);
		glCall(glTexImage2D, GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glCall(glGenerateMipmap, GL_TEXTURE_2D);

		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
