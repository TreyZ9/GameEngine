#pragma once

#include <sstream>
#include <vector>
#include <string>

#include "Model.h"

struct Loader
{
	// accepted flags : model|texture|position|rotation|scale|gamma
	static std::vector<Model> loadModels(const std::string &filename)
	{
		std::ifstream infile(filename);
		std::string line;

		std::vector<Model> models;
		Model activeModel = Model();

		while (std::getline(infile, line))
		{
			if (!line.empty())
			{
				std::vector<std::string> elements = Loader::splitString(line, ":");
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

	static std::vector<std::string> splitString(std::string input, std::string delimiter)
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
};

