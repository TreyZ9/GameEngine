#pragma once

#include <openAL/al.h>
#include <openAL/alc.h>

#include <glm/common.hpp>

class Listener
{
private:
	ALCdevice* device;
	ALCcontext* openALContext;
	ALboolean contextMadeCurrent;
	ALenum distanceModel;
	glm::vec3 position;

public:
	Listener(glm::vec3 position = glm::vec3(0.0f), ALenum distanceModel = AL_LINEAR_DISTANCE_CLAMPED);

	~Listener();

	void updatePosition();
};
