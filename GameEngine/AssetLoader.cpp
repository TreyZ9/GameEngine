
#include "AssetLoader.h"

#include "OpenGLFunctions.h"
#include "stb_image.h"
#include "Debug.h"

#include <fstream>
#include <iostream>

std::map<std::string, Sound> AssetLoader::sounds = {};

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

Texture AssetLoader::loadCubeMap(const std::string& directory)
{
	std::vector<std::string> faces = { directory + "/right.png", directory + "/left.png", directory + "/top.png",
		directory + "/bottom.png", directory + "/back.png", directory + "/front.png" };

	GLuint textureID;
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

			LOG_fileLoad(faces[i], "cubeMap", true);
		}
		else
		{
			stbi_image_free(data);
			LOG_fileLoad(faces[i], "cubeMap", false);
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
	texture.Path = directory;

	return texture;
}

void AssetLoader::cleanUp() {}
