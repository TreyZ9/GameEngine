
#include "Debug.h"

#include <iostream>

void Debug::fileLoad(std::string filename, std::string type, bool success)
{
	if (Debug::debug)
	{
		if (!success)
			std::cout << "[ERROR][0] Could not load file {type:" << type << ", filename:" << filename << "}" << std::endl;
		else
			std::cout << "[DEBUG][0] Loaded file {type:" << type << ", filename:" << filename << "}" << std::endl;
	}
}

const bool Debug::debug = true;
