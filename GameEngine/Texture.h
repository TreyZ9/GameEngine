#pragma once

#include <string>

struct Texture {
	unsigned int id = NULL;
	std::string type;
	std::string path;
	float gamma = NULL;
};
