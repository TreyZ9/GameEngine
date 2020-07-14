#pragma once

#include <openAL\al.h>

#include <string>

struct Sound
{
	std::uint8_t channels;
	std::int32_t sampleRate;
	std::uint8_t bitsPerSample;
	ALsizei dataSize;
	ALenum format;
	char* rawSoundData;
};
