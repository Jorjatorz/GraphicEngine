#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
{
	mName = "Unnamed FrameBuffer";
	//Create it in the constructor
	glGenFramebuffers(1, &mFrameBufferId);
}

FrameBuffer::FrameBuffer(std::string name, int width, int height)
{
	mName = name;
	mWidth = width;
	mHeight = height;
	//Create it in the constructor
	glGenFramebuffers(1, &mFrameBufferId);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &mFrameBufferId);
	//Delete all the textures
	glDeleteTextures(mTexturesIdVector.size(), &mTexturesIdVector.at(0));
}

void FrameBuffer::addTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);

	GLuint newTextureBuffer;
	glGenTextures(1, &newTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, newTextureBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + mTexturesIdVector.size(), GL_TEXTURE_2D, newTextureBuffer, 0);

	mTexturesIdVector.push_back(newTextureBuffer);
}

void FrameBuffer::createDeferredFrameBuffer()
{
	//Color
	addTexture();
	//Normals
	addTexture();
	//Depth and stencil
	glGenRenderbuffers(1, &mRenderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight); //24 bits depth 8 stencil
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferId);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
}

void FrameBuffer::unBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}