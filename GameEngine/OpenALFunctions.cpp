#include "OpenALFunctions.h"

#include <spdlog/spdlog.h>

bool OpenALFunctions::check_al_errors(const std::string& filename, const std::uint_fast32_t line)
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		switch (error)
		{
		case AL_INVALID_NAME:
			spdlog::error("OpenAL Error {} : {}, AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function", filename, line);
			break;
		case AL_INVALID_ENUM:
			spdlog::error("OpenAL Error {} : {}, AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function", filename, line);
			break;
		case AL_INVALID_VALUE:
			spdlog::error("OpenAL Error {} : {}, AL_INVALID_VALUE: an invalid value was passed to an OpenAL function", filename, line);
			break;
		case AL_INVALID_OPERATION:
			spdlog::error("OpenAL Error {} : {}, AL_INVALID_OPERATION: the requested operation is not valid", filename, line);
			break;
		case AL_OUT_OF_MEMORY:
			spdlog::error("OpenAL Error {} : {}, AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory", filename, line);
			break;
		default:
			spdlog::error("OpenAL Error {} : {}, UNKNOWN AL ERROR", filename, line);
		}
		return false;
	}
	return true;
}

bool OpenALFunctions::check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
{
	ALCenum error = alcGetError(device);
	if (error != ALC_NO_ERROR)
	{
		switch (error)
		{
		case ALC_INVALID_VALUE:
			spdlog::error("OpenAL Error {} : {:d}, ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function", filename, line);
			break;
		case ALC_INVALID_DEVICE:
			spdlog::error("OpenAL Error {} : {:d}, ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function", filename, line);
			break;
		case ALC_INVALID_CONTEXT:
			spdlog::error("OpenAL Error {} : {:d}, ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function", filename, line);
			break;
		case ALC_INVALID_ENUM:
			spdlog::error("OpenAL Error {} : {:d}, ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function", filename, line);
			break;
		case ALC_OUT_OF_MEMORY:
			spdlog::error("OpenAL Error {} : {:d}, ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function", filename, line);
			break;
		default:
			spdlog::error("OpenAL Error {} : {:d}, UNKNOWN ALC ERROR", filename, line);
		}
		return false;
	}
	return true;
}
