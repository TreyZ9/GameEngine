#pragma once

#include <glad/glad.h>

#include <string>

#define glCall(function, ...) OpenGLFunctions::glCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)

struct OpenGLFunctions
{
	static bool check_gl_errors(const std::string& filename, const std::uint_fast32_t line);

	template<typename glFunction, typename... Params>
	static auto glCallImpl(const char* filename,
		const std::uint_fast32_t line, glFunction function, Params... params)
		->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>;

	template<typename glFunction, typename... Params>
	static auto glCallImpl(const char* filename,
		const std::uint_fast32_t line, glFunction function, Params... params)
		->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>;
};

template<typename glFunction, typename... Params>
auto OpenGLFunctions::glCallImpl(const char* filename,
	const std::uint_fast32_t line, glFunction function, Params... params)
	->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
	auto ret = function(std::forward<Params>(params)...);
	OpenGLFunctions::check_gl_errors(filename, line);
	return ret;
}

template<typename glFunction, typename... Params>
auto OpenGLFunctions::glCallImpl(const char* filename,
	const std::uint_fast32_t line, glFunction function, Params... params)
	->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
	function(std::forward<Params>(params)...);
	return OpenGLFunctions::check_gl_errors(filename, line);
}
