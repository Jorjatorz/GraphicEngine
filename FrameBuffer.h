#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Definitions.h"

#include <string>
#include <vector>

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(std::string name, int width, int height);
	~FrameBuffer();

	void addTexture();
	void createDeferredFrameBuffer();
	void bind();
	void unBind();

private:
	GLuint mFrameBufferId;
	GLuint mRenderBufferId;

	std::string mName;
	int mWidth, mHeight; //All the frame textures ahs this size (can be change)

	//Holds the textures Id's
	std::vector<GLuint> mTexturesIdVector;
};

#endif