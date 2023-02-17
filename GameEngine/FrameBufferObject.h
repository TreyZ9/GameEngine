#pragma once

#include <glad/glad.h>

#include <glm/common.hpp>

class FrameBufferObject
{
private:
	GLuint fbo;
	GLuint rbo;

public:
	GLuint textureColorID;

	FrameBufferObject();

	~FrameBufferObject();

	void bind();

	void unbind(glm::ivec2 resolution);

	void destroy();
};
