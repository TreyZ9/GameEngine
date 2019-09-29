#pragma once

#include "Model.h"

struct ModelLoader
{
	// accepted flags : model|texture|position|rotation|scale|gamma
	static std::vector<Model> loadModels(const std::string& filename);

	static std::vector<std::string> splitString(std::string input, std::string delimiter);
};

