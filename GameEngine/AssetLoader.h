#pragma once

#include "Texture.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>
#include <vector>
#include <map>

struct AssetLoader
{
public:
	static std::map<std::string, Texture> textures;
	static std::vector<GLuint> vaos;
	static std::vector<GLuint> vbos;
	static std::vector<GLuint> ebos;

	static Texture loadTexture(const std::string& filename, const std::string& type, bool gamma = false);
	static Texture loadCubeMap(const std::string& directory);
	static void cleanUp();

	template <typename dataType> static void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<dataType> data);
	template <typename dataSize_t, typename offset_t> static void createAttibutePointer(int attributeNumber, int coordinateSize, dataSize_t dataType, offset_t offset);

	static GLuint bindIndicesArray(std::vector<GLuint> indices);
	static GLuint createVAO();
	static void unbindVAO();
};

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

