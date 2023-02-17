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

	Source(const std::string& filename, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& velocity = glm::vec3(0.0f),
		const ALfloat pitch = 1.0f, const ALfloat gain = 1.0f, const ALfloat referenceDistance = 0.0f, const ALfloat maxDistance = 10.0f,
		const ALfloat rolloffFactor = 1.0f, const ALboolean looping = AL_FALSE);

	~Source();

	void play();

	void pause();

	void stop();

	void setPosition(const glm::vec3& position);
};
