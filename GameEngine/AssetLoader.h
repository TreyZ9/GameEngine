#pragma once

#include "Texture.h"
#include "Sound.h"
#include "Model.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>
#include <vector>
#include <map>
//#include <bit>

struct AssetLoader
{
public:
	static std::map<std::string, Sound> sounds;

	static std::int32_t convertToInt(char* buffer, std::size_t len);
	static bool loadWavFileHeader(std::ifstream& file, Sound& sound);
	static Sound loadWav(const std::string &filename);

	static Texture loadCubeMap(const std::string& directory);
	static void cleanUp();
};
