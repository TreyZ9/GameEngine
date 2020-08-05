#include "OpenGLFunctions.h"

bool OpenGLFunctions::check_gl_errors(const std::string& filename, const std::uint_fast32_t line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] (" << filename << ": " << line << ")" << std::endl;
		switch (error)
		{
		case GL_INVALID_ENUM:
			std::cerr << "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending "
				<< "command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cerr << "GL_INVALID_VALUE: A numeric argument is out of range.The offending command is ignored and "
				<< "has no other side effect than to set the error flag." << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cerr << "GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The "
				<< "offending command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending "
				<< "command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL "
				<< "is undefined, except for the state of the error flags, after this error is recorded." << std::endl;
			break;
		case GL_STACK_UNDERFLOW:
			std::cerr << "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an "
				<< "internal stack to underflow." << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			std::cerr << "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an "
				<< "internal stack to overflow." << std::endl;
			break;
		default:
			std::cerr << "GL_UNKNOWN_ERROR: Idk, this should have been impossible." << std::endl;
			break;
		}
		return false;
	}
	return true;
}
