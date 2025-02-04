#include "Source.h"

#include <spdlog/spdlog.h>

#include "OpenALFunctions.h"
#include "Loader.h"

Source::Source(const std::string& filename, const glm::vec3& position, const glm::vec3& velocity,
	const ALfloat pitch, const ALfloat gain, const ALfloat referenceDistance, const ALfloat maxDistance,
	const ALfloat rolloffFactor, const ALboolean looping) :
	position(position), velocity(velocity), pitch(pitch), gain(gain), referenceDistance(referenceDistance),
	maxDistance(maxDistance), rolloffFactor(rolloffFactor), looping(looping)
{
	this->sound = Loader::loadWav(filename);
	if (this->sound.RawSoundData == nullptr || this->sound.DataSize == 0)
	{
		spdlog::error("Loaded audio file '{}'", filename);
	}
	alCall(alGenBuffers, 1, &this->buffer);

	ALenum format;
	if (sound.Channels == 1 && sound.BitsPerSample == 8)
	{
		format = AL_FORMAT_MONO8;
	}
	else if (sound.Channels == 1 && sound.BitsPerSample == 16)
	{
		format = AL_FORMAT_MONO16;
	}
	else if (sound.Channels == 2 && sound.BitsPerSample == 8)
	{
		format = AL_FORMAT_STEREO8;
	}
	else if (sound.Channels == 2 && sound.BitsPerSample == 16)
	{
		format = AL_FORMAT_STEREO16;
	}
	else
	{
		spdlog::error("Unrecognized wave format '{}'", filename);
		format = NULL;
	}

	std::vector<char> soundData(this->sound.RawSoundData, this->sound.RawSoundData + this->sound.DataSize);
	alCall(alBufferData, buffer, format, soundData.data(), soundData.size(), sound.SampleRate);
	soundData.clear();

	alCall(alGenSources, 1, &this->id);
	alCall(alSourcef, this->id, AL_PITCH, this->pitch);
	alCall(alSourcef, this->id, AL_GAIN, this->gain);
	alCall(alSource3f, this->id, AL_POSITION, this->position.x, this->position.y, this->position.z);
	alCall(alSource3f, this->id, AL_VELOCITY, this->velocity.x, this->velocity.y, this->velocity.z);
	alCall(alSourcei, this->id, AL_LOOPING, this->looping);
	alCall(alSourcef, this->id, AL_ROLLOFF_FACTOR, this->rolloffFactor);
	alCall(alSourcef, this->id, AL_REFERENCE_DISTANCE, this->referenceDistance);
	alCall(alSourcef, this->id, AL_MAX_DISTANCE, this->maxDistance);
	alCall(alSourcei, this->id, AL_BUFFER, buffer);

	this->state = AL_FALSE;
}

Source::~Source() 
{
	alCall(alDeleteSources, 1, &this->id);
	alCall(alDeleteBuffers, 1, &this->buffer);
}

void Source::play()
{
	alCall(alSourcePlay, this->id);
}

void Source::pause()
{
	alCall(alSourcePause, this->id);
}

void Source::stop()
{
	alCall(alSourceStop, this->id);
}

void Source::setPosition(const glm::vec3& position)
{
	this->position = position;
	alCall(alSource3f, this->id, AL_POSITION, this->position.x, this->position.y, this->position.z);
}
