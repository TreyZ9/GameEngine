#include "FrameBufferObject.h"

#include <spdlog/spdlog.h>

#include "OpenGLFunctions.h"
#include "Config.h"

FrameBufferObject::FrameBufferObject()
{
	glCall(glGenFramebuffers, 1, &this->fbo);
	glCall(glBindFramebuffer, GL_FRAMEBUFFER, this->fbo);

	glCall(glGenTextures, 1, &this->textureColorID);
	glCall(glBindTexture, GL_TEXTURE_2D, this->textureColorID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // Currently Incompatible With glCall, Needs Fixed
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glCall(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureColorID, 0);

	glCall(glGenRenderbuffers, 1, &this->rbo);
	glCall(glBindRenderbuffer, GL_RENDERBUFFER, this->rbo);
	glCall(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
	glCall(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::error("Framebuffer incomplete");
	}

	glCall(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}

FrameBufferObject::~FrameBufferObject() {}

void FrameBufferObject::bind()
{
	glCall(glBindFramebuffer, GL_FRAMEBUFFER, this->fbo);
	glCall(glClearColor, 0.0f, 1.0f, 1.0f, 1.0f);
	glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCall(glViewport, 0, 0, 1024, 1024);
}

void FrameBufferObject::unbind()
{
	glCall(glBindFramebuffer, GL_FRAMEBUFFER, 0);
	glCall(glViewport, 0, 0, Config::Display::WIDTH, Config::Display::HEIGHT);
}

void FrameBufferObject::destroy()
{
	glCall(glDeleteFramebuffers, 1, &this->fbo);
	glCall(glDeleteTextures, 1, &this->textureColorID);
	glCall(glDeleteRenderbuffers, 1, &this->rbo);
}