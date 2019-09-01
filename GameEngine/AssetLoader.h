#pragma once
#include "Texture.h"

struct AssetLoader
{
public:
	static std::map<std::string, Texture> textures;
	static std::vector<GLuint> vaos;
	static std::vector<GLuint> vbos;
	static std::vector<GLuint> ebos;

	static Texture loadTexture(const std::string &filename, const std::string &type, bool gamma = false);
	static Texture loadCubeMap(const std::string &directory);
	static void cleanUp();

	template <typename dataType> static void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<dataType> data);
	template <typename dataSize_t, typename offset_t> static void createAttibutePointer(int attributeNumber, int coordinateSize, dataSize_t dataType, offset_t offset);

	static GLuint bindIndicesArray(std::vector<GLuint> indices);
	static GLuint createVAO();
	static void unbindVAO();
};

std::map<std::string, Texture> AssetLoader::textures = {};
std::vector<GLuint> AssetLoader::vaos = {};
std::vector<GLuint> AssetLoader::vbos = {};
std::vector<GLuint> AssetLoader::ebos = {};

Texture AssetLoader::loadTexture(const std::string &filename, const std::string &type, bool gamma)
{
	if (AssetLoader::textures.count(filename) == 0)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);

			Debug::fileLoad(filename, type, true);
		}
		else
		{
			Debug::fileLoad(filename, type, false);
			stbi_image_free(data);
		}

		Texture texture;
		texture.id = textureID;
		texture.type = type;
		texture.path = filename;
		texture.gamma = gamma;
		AssetLoader::textures.insert(std::pair<std::string, Texture>(filename, texture));

		return texture;
	}
	else
		return AssetLoader::textures[filename];
}

Texture AssetLoader::loadCubeMap(const std::string &directory)
{
	if (AssetLoader::textures.count(directory) == 0)
	{
		std::vector<std::string> faces = { directory + "/right.png", directory + "/left.png", directory + "/top.png",
			directory + "/bottom.png", directory + "/back.png", directory + "/front.png" };

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		stbi_set_flip_vertically_on_load(false);
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			int width, height, nrChannels;
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);

				Debug::fileLoad(faces[i], "cubeMap", true);
			}
			else
			{
				stbi_image_free(data);
				Debug::fileLoad(faces[i], "cubeMap", false);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		Texture texture;
		texture.id = textureID;
		texture.type = "cubeMap";
		texture.path = directory;
		texture.gamma = 0.0f;
		AssetLoader::textures.insert(std::pair<std::string, Texture>(directory, texture));

		return texture;
	}
	else
		return AssetLoader::textures[directory];
}

void AssetLoader::cleanUp()
{

	for (GLuint vao : AssetLoader::vaos)
		glDeleteVertexArrays(1, &vao);
	for (GLuint vbo : AssetLoader::vbos)
		glDeleteBuffers(1, &vbo);
	for (GLuint ebo : AssetLoader::ebos)
		glDeleteBuffers(1, &ebo);
	for (auto const &pair : AssetLoader::textures)
		glDeleteTextures(1, &pair.second.id);
}

template <typename dataType_t> void AssetLoader::storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<dataType_t> data)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(dataType_t), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, 0);
}

template <typename dataSize_t, typename offset_t> void AssetLoader::createAttibutePointer(int attributeNumber, int coordinateSize, dataSize_t dataSize, offset_t offset)
{
	glEnableVertexAttribArray(attributeNumber);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, dataSize, offset);
}

GLuint AssetLoader::bindIndicesArray(std::vector<GLuint> indices) {
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);
	return EBO;
}

GLuint AssetLoader::createVAO() {
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void AssetLoader::unbindVAO() { glBindVertexArray(0); }