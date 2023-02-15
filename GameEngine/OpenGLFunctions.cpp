#include "OpenGLFunctions.h"

#include <spdlog/spdlog.h>

bool OpenGLFunctions::check_gl_errors(const std::string& filename, const std::uint_fast32_t line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			spdlog::error("OpenGL Error {} : {:d}, GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag", filename, line);
			break;
		case GL_INVALID_VALUE:
			spdlog::error("OpenGL Error {} : {:d}, GL_INVALID_VALUE: A numeric argument is out of range.The offending command is ignored and has no other side effect than to set the error flag", filename, line);
			break;
		case GL_INVALID_OPERATION:
			spdlog::error("OpenGL Error {} : {:d}, GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag", filename, line);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			spdlog::error("OpenGL Error {} : {:d}, GL_INVALID_FRAMEBUFFER_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag", filename, line);
			break;
		case GL_OUT_OF_MEMORY:
			spdlog::error("OpenGL Error {} : {:d}, GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded", filename, line);
			break;
		case GL_STACK_UNDERFLOW:
			spdlog::error("OpenGL Error {} : {:d}, GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow", filename, line);
			break;
		case GL_STACK_OVERFLOW:
			spdlog::error("OpenGL Error {} : {:d}, GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow", filename, line);
			break;
		default:
			spdlog::error("OpenGL Error {} : {:d}, GL_UNKNOWN_ERROR: An undefined error with undefined consequences", filename, line);
			break;
		}
		return false;
	}
	return true;
}
