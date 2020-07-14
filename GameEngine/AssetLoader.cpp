
#include "AssetLoader.h"

#include "stb_image.h"
#include "Debug.h"

#include <fstream>
#include <iostream>

std::map<std::string, Texture> AssetLoader::textures = {};
std::map<std::string, Sound> AssetLoader::sounds = {};
std::vector<GLuint> AssetLoader::vaos = {};
std::vector<GLuint> AssetLoader::vbos = {};
std::vector<GLuint> AssetLoader::ebos = {};

// accepted flags : model|texture|position|rotation|scale|gamma
std::vector<Model> AssetLoader::loadModels(const std::string& filename)
{
	std::ifstream infile(filename);
	std::string line;

	std::vector<Model> models;
	Model activeModel = Model();

	while (std::getline(infile, line))
	{
		if (!line.empty())
		{
			std::vector<std::string> elements = AssetLoader::splitString(line, ":");
			if (elements[0] == "model")
			{
				if (elements[1] != "end")
					activeModel = Model(elements[1]);
				else
					models.push_back(activeModel);
			}
			if (elements[0] == "texture")
				activeModel.meshes[0].applyTexture(elements[1], elements[2]);
			if (elements[0] == "position")
				activeModel.setPosition(glm::vec3(std::stof(elements[1]), std::stof(elements[2]), std::stof(elements[3])));
			if (elements[0] == "rotation")
				activeModel.setRotation(glm::vec3(std::stof(elements[1]), std::stof(elements[2]), std::stof(elements[3])));
			if (elements[0] == "scale")
				activeModel.setScale(std::stof(elements[1]));
			if (elements[0] == "gamma")
			{
				if (elements[1] == "true")
					activeModel.meshes[0].gamma = std::stof(elements[2]);
				else
					activeModel.meshes[0].gamma = 1;
			}
		}
	}

	return models;
}

std::vector<std::string> AssetLoader::splitString(std::string input, std::string delimiter)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	while (pos < input.length() && prev < input.length())
	{
		pos = input.find(delimiter, prev);
		if (pos == std::string::npos)
			pos = input.length();
		std::string token = input.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delimiter.length();
	}

	return tokens;
}

std::int32_t AssetLoader::convertToInt(char* buffer, std::size_t len)
{
	std::int32_t a = 0;
	if (std::endian::native == std::endian::little)
		std::memcpy(&a, buffer, len);
	else
		for (std::size_t i = 0; i < len; ++i)
			reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
	return a;
}

bool AssetLoader::loadWavFileHeader(std::ifstream& file, Sound& sound)
{
	char buffer[4];
	if (!file.is_open())
		return false;

	// the RIFF
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read RIFF" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
		return false;
	}

	// the size of the file
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read size of file" << std::endl;
		return false;
	}

	// the WAVE
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read WAVE" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "WAVE", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
		return false;
	}

	// "fmt/0"
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read fmt/0" << std::endl;
		return false;
	}

	// this is always 16, the size of the fmt data chunk
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read the 16" << std::endl;
		return false;
	}

	// PCM should be 1?
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read PCM" << std::endl;
		return false;
	}

	// the number of channels
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read number of channels" << std::endl;
		return false;
	}
	sound.channels = AssetLoader::convertToInt(buffer, 2);

	// sample rate
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read sample rate" << std::endl;
		return false;
	}
	sound.sampleRate = AssetLoader::convertToInt(buffer, 4);

	// (sampleRate * bitsPerSample * channels) / 8
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
		return false;
	}

	// ?? dafaq
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read dafaq" << std::endl;
		return false;
	}

	// bitsPerSample
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read bits per sample" << std::endl;
		return false;
	}
	sound.bitsPerSample = AssetLoader::convertToInt(buffer, 2);

	// data chunk header "data"
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read data chunk header" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "data", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
		return false;
	}

	// size of data
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read data size" << std::endl;
		return false;
	}
	sound.dataSize = AssetLoader::convertToInt(buffer, 4);

	/* cannot be at the end of file */
	if (file.eof())
	{
		std::cerr << "ERROR: reached EOF on the file" << std::endl;
		return false;
	}
	if (file.fail())
	{
		std::cerr << "ERROR: fail state set on the file" << std::endl;
		return false;
	}

	return true;
}

Sound AssetLoader::loadWav(const std::string& filename)
{
	if (AssetLoader::sounds.count(filename) == 0)
	{
		Sound sound;

		std::ifstream in(filename, std::ios::binary);
		if (!in.is_open())
		{
			std::cerr << "ERROR: Could not open \"" << filename << "\"" << std::endl;
			exit(-1);
		}
		if (!AssetLoader::loadWavFileHeader(in, sound))
		{
			std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"" << std::endl;
			exit(-1);
		}

		sound.rawSoundData = new char[sound.dataSize];

		in.read(sound.rawSoundData, sound.dataSize);

		AssetLoader::sounds.insert(std::pair<std::string, Sound>(filename, sound));

		return sound;
	}
	else
	{
		return AssetLoader::sounds[filename];
	}
}

Texture AssetLoader::loadTexture(const std::string& filename, const std::string& type, bool gamma)
{
	if (AssetLoader::textures.count(filename) == 0)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

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

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);

			LOG_fileLoad(filename, type, true);
		}
		else
		{
			LOG_fileLoad(filename, type, false);
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

Texture AssetLoader::loadCubeMap(const std::string& directory)
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
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);

				LOG_fileLoad(faces[i], "cubeMap", true);
			}
			else
			{
				stbi_image_free(data);
				LOG_fileLoad(faces[i], "cubeMap", false);
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
	for (auto const& pair : AssetLoader::textures)
		glDeleteTextures(1, &pair.second.id);
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
