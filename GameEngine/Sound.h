#pragma once

#include <openAL\al.h>

#include <string>

struct Sound
{
	std::uint8_t Channels;
	std::int32_t SampleRate;
	std::uint8_t BitsPerSample;
	ALsizei DataSize;
	ALenum Format;
	char* RawSoundData;
};
