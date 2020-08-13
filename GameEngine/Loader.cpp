#include "Loader.h"

#include "stb_image.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <fstream>

#include "OpenGLFunctions.h"
#include "OpenALFunctions.h"

std::map<std::string, Sound> Loader::sounds;
std::map<std::string, Texture> Loader::textures;
std::vector<GLuint> Loader::vaos;
std::vector<GLuint> Loader::vbos;
std::vector<GLuint> Loader::ebos;

void Loader::loadSceneJSON(const std::string& filename)
{
	std::ifstream jsonFile(filename);
	rapidjson::IStreamWrapper jsonFileWrapped(jsonFile);

	rapidjson::Document jsonDocument;
	jsonDocument.ParseStream(jsonFileWrapped);

	const rapidjson::Value& shaders = jsonDocument["Shaders"];
	for (rapidjson::Value::ConstMemberIterator shader = shaders.MemberBegin(); shader != shaders.MemberEnd(); shader++)
	{
		std::cout << shader->name.GetString() << std::endl;
		std::cout << "   " << shader->value["VertexPath"].GetString() << std::endl;
		std::cout << "   " << shader->value["FragmentPath"].GetString() << std::endl;
	}
}

std::int32_t Loader::convertToInt(char* buffer, std::size_t len)
{
	std::int32_t a = 0;
	if (std::endian::native == std::endian::little)
		std::memcpy(&a, buffer, len);
	else
		for (std::size_t i = 0; i < len; ++i)
			reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
	return a;
}

bool Loader::loadWavFileHeader(std::ifstream& file, Sound& sound)
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
	sound.channels = Loader::convertToInt(buffer, 2);

	// sample rate
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read sample rate" << std::endl;
		return false;
	}
	sound.sampleRate = Loader::convertToInt(buffer, 4);

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
	sound.bitsPerSample = Loader::convertToInt(buffer, 2);

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
	sound.dataSize = Loader::convertToInt(buffer, 4);

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

Sound Loader::loadWav(const std::string& filename)
{
	if (Loader::sounds.count(filename) == 0)
	{
		Sound sound;

		std::ifstream in(filename, std::ios::binary);
		if (!in.is_open())
		{
			std::cerr << "ERROR: Could not open \"" << filename << "\"" << std::endl;
			exit(-1);
		}
		if (!Loader::loadWavFileHeader(in, sound))
		{
			std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"" << std::endl;
			exit(-1);
		}

		sound.rawSoundData = new char[sound.dataSize];

		in.read(sound.rawSoundData, sound.dataSize);

		Loader::sounds.insert(std::pair<std::string, Sound>(filename, sound));

		return sound;
	}
	else
		return Loader::sounds[filename];
}

GLuint Loader::createVAO()
{
	unsigned int vaoID;
	glCall(glGenVertexArrays, 1, &vaoID);
	glCall(glBindVertexArray, vaoID);
	Loader::vaos.push_back(vaoID);
	return vaoID;
}

GLuint Loader::createEBO(std::vector<GLuint> indices)
{
	GLuint eboID;
	glCall(glGenBuffers, 1, &eboID);
	glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, eboID);
	glCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	Loader::ebos.push_back(eboID);
	return eboID;
}

void Loader::unbindVAO()
{
	glCall(glBindVertexArray, 0);
}

Texture Loader::loadTexture(const std::string& filename, const std::string& typeName)
{
	if (Loader::textures.count(filename) == 0)
	{
		GLuint textureID;
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

		Loader::textures.insert(std::pair(filename, texture));

		return texture;
	}
	else
		return Loader::textures[filename];
}

Texture Loader::loadCubeMap(const std::string& path)
{
	std::vector<std::string> faces = { path + "/right.png", path + "/left.png", path + "/top.png",
			path + "/bottom.png", path + "/back.png", path + "/front.png" };

	if (Loader::textures.count(path) == 0)
	{
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

		Loader::textures.insert(std::pair(path, texture));

		return texture;
	}
	else
		return Loader::textures[path];
}

Texture Loader::loadTextureFromPath(const std::string& path, const std::string& directory, const std::string& type, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	if (Loader::textures.count(filename) == 0)
	{
		GLuint textureID;
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

		Texture texture;
		texture.ID = textureID;
		texture.Path = path;
		texture.Type = type;

		return texture;
	}
	else
		return Loader::textures[filename];
}

void Loader::destroy()
{
	for (GLuint vao : Loader::vaos)
		glCall(glDeleteVertexArrays, 1, &vao);
	for (GLuint vbo : Loader::vbos)
		glCall(glDeleteBuffers, 1, &vbo);
	for (GLuint ebo : Loader::ebos)
		glCall(glDeleteBuffers, 1, &ebo);
	for (std::pair<std::string, Texture> texture : Loader::textures)
		glCall(glDeleteTextures, 1, &texture.second.ID);
}
