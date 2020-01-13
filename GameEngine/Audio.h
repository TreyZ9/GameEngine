#pragma once

#include <string>

#include <openAL/al.h>
#include <openAL/alc.h>

struct Audio
{
	unsigned int sourceID, bufferID;
	ALCdevice* device;
	ALCcontext* context;
	char* data;

	Audio();
	~Audio();

	bool isBigEndian();
	int convertToInt(char* buffer, int len);
	char* loadWAV(std::string filename, int& chan, int& sampleRate, int& bps, int& size);

	void cleanUp();
};
