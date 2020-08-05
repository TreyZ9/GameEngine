#pragma once

#include <glad/glad.h>

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
	void unbind();
};

