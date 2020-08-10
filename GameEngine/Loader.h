#pragma once

#include <vector>
#include <string>

#include "OpenGLFunctions.h"
#include "Texture.h"

struct Loader
{
	static unsigned int createVAO();
	static unsigned int bindIndicesArray(std::vector<unsigned int> indices);
	static void unbindVAO();

	static Texture loadTexture(const std::string& filename, const std::string& typeName);
	static Texture loadCubeMap(const std::string& path);

	static unsigned int loadTextureFromPath(const char* path, const std::string& directory, bool gamma = false);

	template <typename dataType> static void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<dataType> data);
	template <typename dataSize_t, typename offset_t> static void createAttibutePointer(int attributeNumber, int coordinateSize, dataSize_t dataType, offset_t offset);
};

template <typename dataType_t> void Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<dataType_t> data)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(dataType_t), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, 0);
}

template <typename dataSize_t, typename offset_t> void Loader::createAttibutePointer(int attributeNumber, int coordinateSize, dataSize_t dataSize, offset_t offset)
{
	glEnableVertexAttribArray(attributeNumber);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, (GLsizei)dataSize, offset);
}
