#pragma once

#include "Sound.h"

#include <openAL/al.h>

#include <vector>
#include <string>

#include <glm/common.hpp>

class Source
{
private:
	glm::vec3 position;
	glm::vec3 velocity;
	ALfloat pitch;
	ALfloat gain;
	ALfloat referenceDistance;
	ALfloat maxDistance;
	ALfloat rolloffFactor;
	ALboolean looping;

	Sound sound;
	ALuint buffer;
	ALuint id;
	ALint state;

public:
	Source() = default;

	Source(const std::string filename, glm::vec3 position = glm::vec3(0.0f), glm::vec3 velocity = glm::vec3(0.0f),
		ALfloat pitch = 1.0f, ALfloat gain = 1.0f, ALfloat referenceDistance = 0.0f, ALfloat maxDistance = 10.0f,
		ALfloat rolloffFactor = 1.0f, ALboolean looping = AL_FALSE);

	~Source();

	void play();

	void pause();

	void stop();

	void setPosition(glm::vec3 position);
};
