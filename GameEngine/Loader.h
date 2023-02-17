#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>
#include <map>

#include "OpenGLFunctions.h"
#include "Texture.h"
#include "Model.h"
#include "Sound.h"

struct Loader
{
	static std::map<std::string, Sound> sounds;
	static std::map<std::string, Texture> textures;
	static std::vector<GLuint> vaos;
	static std::vector<GLuint> vbos;
	static std::vector<GLuint> ebos;

	static void loadSceneJSON(const std::string& filename);

	static std::int32_t convertToInt(char* buffer, std::size_t len);

	static bool loadWavFileHeader(std::ifstream& file, Sound& sound);

	static Sound loadWav(const std::string& filename);

	static GLuint createVAO();

	static GLuint createEBO(std::vector<GLuint> indices);

	static void unbindVAO();

	static Texture loadTexture(const std::string& filename, const std::string& typeName);

	static Texture loadCubeMap(const std::string& path);

	static Texture createEmptyCubeMap();

	static Texture loadTextureFromPath(const std::string& path, const std::string& directory, const std::string& type, bool gamma = false);

	template <typename dataType> static void storeDataInAttributeList(GLuint attributeNumber, GLuint coordinateSize, std::vector<dataType> data);

	template <typename dataSize_t, typename offset_t> static void createAttibutePointer(GLuint attributeNumber, GLuint coordinateSize, dataSize_t dataType, offset_t offset);

	static void destroy();
};

template <typename dataType_t> void Loader::storeDataInAttributeList(GLuint attributeNumber, GLuint coordinateSize, std::vector<dataType_t> data)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(dataType_t), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, 0);
}

template <typename dataSize_t, typename offset_t> void Loader::createAttibutePointer(GLuint attributeNumber, GLuint coordinateSize, dataSize_t dataSize, offset_t offset)
{
	glEnableVertexAttribArray(attributeNumber);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, (GLsizei)dataSize, offset);
}
