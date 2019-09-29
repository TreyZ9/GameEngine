#pragma once

#include <string>

struct Debug
{
private:
	const static bool debug;
public:
	static void fileLoad(std::string filename, std::string type, bool success);
};
