#pragma once

#include <openAL/al.h>
#include <openAL/alc.h>

#include <string>

#define alCall(function, ...) OpenALFunctions::alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) OpenALFunctions::alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

struct OpenALFunctions
{
	static bool check_al_errors(const std::string& filename, const std::uint_fast32_t line);

	static bool check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);

	template<typename alFunction, typename... Params>
	static auto alCallImpl(const char* filename,
		const std::uint_fast32_t line, alFunction function, Params... params)
		->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>;

	template<typename alFunction, typename... Params>
	static auto alCallImpl(const char* filename,
		const std::uint_fast32_t line, alFunction function, Params... params)
		->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>;

	template<typename alcFunction, typename... Params>
	static auto alcCallImpl(const char* filename,
		const std::uint_fast32_t line, alcFunction function, ALCdevice* device, Params... params)
		->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>;

	template<typename alcFunction, typename ReturnType, typename... Params>
	static auto alcCallImpl(const char* filename,
		const std::uint_fast32_t line, alcFunction function, ReturnType& returnValue, ALCdevice* device, Params... params)
		->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>;
};

template<typename alFunction, typename... Params>
auto OpenALFunctions::alCallImpl(const char* filename,
	const std::uint_fast32_t line, alFunction function, Params... params)
	->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
	auto ret = function(std::forward<Params>(params)...);
	OpenALFunctions::check_al_errors(filename, line);
	return ret;
}

template<typename alFunction, typename... Params>
auto OpenALFunctions::alCallImpl(const char* filename,
	const std::uint_fast32_t line, alFunction function, Params... params)
	->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
	function(std::forward<Params>(params)...);
	return OpenALFunctions::check_al_errors(filename, line);
}

template<typename alcFunction, typename... Params>
auto OpenALFunctions::alcCallImpl(const char* filename,
	const std::uint_fast32_t line, alcFunction function, ALCdevice* device, Params... params)
	->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
	function(std::forward<Params>(params)...);
	return OpenALFunctions::check_alc_errors(filename, line, device);
}

template<typename alcFunction, typename ReturnType, typename... Params>
auto OpenALFunctions::alcCallImpl(const char* filename,
	const std::uint_fast32_t line, alcFunction function, ReturnType& returnValue, ALCdevice* device, Params... params)
	->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>
{
	returnValue = function(std::forward<Params>(params)...);
	return OpenALFunctions::check_alc_errors(filename, line, device);
};
