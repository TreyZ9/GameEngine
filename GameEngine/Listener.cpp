#include "Listener.h"

#include "OpenALFunctions.h"
#include "Camera.h"
#include "Maths.h"

#include <spdlog/spdlog.h>

Listener::Listener(glm::vec3 position, ALenum distanceModel) : 
	position(position), distanceModel(distanceModel)
{
	this->device = alcOpenDevice(nullptr);
	if (!this->device)
	{
		spdlog::error("Could not open audio device");
	}
	else
	{
		spdlog::debug("Opened audio device");
	}

	if (!alcCall(alcCreateContext, this->openALContext, this->device, this->device, nullptr) || !this->openALContext)
	{
		spdlog::error("Could not create alc context");
	}
	else
	{
		spdlog::debug("Created alc context");
	}

	this->contextMadeCurrent = false;
	if (!alcCall(alcMakeContextCurrent, this->contextMadeCurrent, this->device, this->openALContext)
		|| this->contextMadeCurrent != ALC_TRUE)
	{
		spdlog::error("Could not make alc context current");
	}
	else
	{
		spdlog::debug("Made alc context current");
	}

	alCall(alDistanceModel, AL_LINEAR_DISTANCE_CLAMPED);
}

Listener::~Listener() 
{
	alcCall(alcMakeContextCurrent, this->contextMadeCurrent, this->device, nullptr);
	alcCall(alcDestroyContext, this->device, this->openALContext);
	ALCboolean closed;
	alcCall(alcCloseDevice, closed, this->device, this->device);
}

void Listener::updatePosition()
{
	glm::mat4 alViewMatrix;
	Maths::createViewMatrixAL(alViewMatrix);
	ALfloat orientation[] = {
		alViewMatrix[0][2], alViewMatrix[1][2], alViewMatrix[2][2],  // Look vector
		alViewMatrix[0][1], alViewMatrix[1][1], alViewMatrix[2][1] }; // Up Vector
	ALfloat position[] = {
		Camera::position.x, Camera::position.y, Camera::position.z };
	alCall(alListenerfv, AL_POSITION, position);
	alCall(alListenerfv, AL_ORIENTATION, orientation);
}