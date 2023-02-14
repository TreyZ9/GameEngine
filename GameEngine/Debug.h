#pragma once

#include <string>

#define LOG_fileLoad(loadedFile, type, success) Debug::fileLoad(__FILE__, __LINE__, loadedFile, type, success)
#define LOG_fileLoadInfo(loadedFile, type, success, info) Debug::fileLoad(__FILE__, __LINE__, loadedFile, type, success, info)
#define LOG_alcOpenDevice(success) Debug::alcOpenDevice(__FILE__, __LINE__, success)
#define LOG_alcCreateContext(success) Debug::alcCreateContext(__FILE__, __LINE__, success)
#define LOG_alcMakeContextCurrent(success) Debug::alcMakeContextCurrent(__FILE__, __LINE__, success)
#define LOG_freetypeLibraryLoad(success) Debug::freetypeLibraryLoad(__FILE__, __LINE__, success);
#define LOG_freetypeFontLoad(success, info) Debug::freetypeFontLoad(__FILE__, __LINE__, success, info);
#define LOG_freetypeCharacterLoad(success, info) Debug::freetypeCharacterLoad(__FILE__, __LINE__, success, info);
#define LOG_freetypeGlyphLoad(success, info) Debug::freetypeGlyphLoad(__FILE__, __LINE__, success, info);
#define LOG_freetypeRenderBitmap(success, info) Debug::freetypeRenderBitmap(__FILE__, __LINE__, success, info);

struct Debug
{
private:
	const static bool debug;

public:
	static void fileLoad(const char* filename, const std::uint_fast32_t line, 
		std::string loadedFile, std::string type, bool success, std::string info = "");

	static void alcOpenDevice(const char* filename, const std::uint_fast32_t line,
		bool success, std::string info = "");

	static void alcCreateContext(const char* filename, const std::uint_fast32_t line,
		bool success, std::string info = "");

	static void alcMakeContextCurrent(const char* filename, const std::uint_fast32_t line,
		bool success, std::string info = "");

	static void freetypeLibraryLoad(const char* filename, const std::uint_fast32_t line,
		bool success);

	static void freetypeFontLoad(const char* filename, const std::uint_fast32_t line,
		bool success, std::string info = "");

	static void freetypeCharacterLoad(const char* filename, const std::uint_fast32_t line,
		bool success, std::string info = "");

	static void freetypeGlyphLoad(const char* filename, const std::uint_fast32_t line,
		bool success, std::string info = "");

	static void freetypeRenderBitmap(const char* filename, const std::uint_fast32_t line,
		bool success, std::string info = "");
};
