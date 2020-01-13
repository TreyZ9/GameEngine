
#include "Audio.h"

#include <iostream>
#include <fstream>
#include <string>

#include <openAL/al.h>
#include <openAL/alc.h>

Audio::Audio()
{
	int channel, sampleRate, bps, size;
	char *data = this->loadWAV("res/audio/ambient.wav", channel, sampleRate, bps, size);
	this->data = data;

	ALCdevice *device = alcOpenDevice(NULL);
	if (device == NULL)
		std::cout << "Cannot Open Sound Device" << std::endl;

	ALCcontext* context = alcCreateContext(device, NULL);
	if (context == NULL)
		std::cout << "Cannot Open Context" << std::endl;
	alcMakeContextCurrent(context);

	unsigned int bufferID, format;
	alGenBuffers(1, &bufferID);
	if (channel == 1)
	{
		if (bps == 8)
			format = AL_FORMAT_MONO8;
		else
			format = AL_FORMAT_MONO16;
	}
	else
	{
		if (bps == 8)
			format = AL_FORMAT_STEREO8;
		else
			format = AL_FORMAT_STEREO16;
	}

	alBufferData(bufferID, format, data, size, sampleRate);
	unsigned int sourceID;
	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);

	std::cout << "Playing" << std::endl;
	ALint sourceState;
	alSourcePlay(sourceID);
	alGetSourcei(sourceID, AL_SOURCE_STATE, &sourceState);
	while (sourceState == AL_PLAYING)
		alGetSourcei(sourceID, AL_SOURCE_STATE, &sourceState);
	std::cout << "End" << std::endl;

	this->sourceID = sourceID;
	this->bufferID = bufferID;
	this->context = context;
}

Audio::~Audio() {}

bool Audio::isBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

int Audio::convertToInt(char *buffer, int len)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i < len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

char* Audio::loadWAV(std::string filename, int &channels, int &sampleRate, int &bps, int &size)
{
	char buffer[4];
	std::ifstream in(filename.c_str(), std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "This is not a WAV file" << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	channels = convertToInt(buffer, 2);
	in.read(buffer, 4);
	sampleRate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4);
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);

	return data;
}

void Audio::cleanUp()
{
	alDeleteSources(1, &this->sourceID);
	alDeleteBuffers(1, &this->bufferID);

	alcDestroyContext(this->context);
	//alcCloseDevice(this->device);

	delete[] this->data;
}
