#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Definitions.h"

#include <string>
#include <vector>

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(std::string name, int width, int height, SceneManager* manager);
	~FrameBuffer();

	void addTexture(int format);
	void createGBuffer();
	void bindForDrawing();
	void bindForReading();
	void bindForRendering();
	void unBind();

private:
	GLuint mFrameBufferId;
	GLuint mRenderBufferId;

	std::string mName;
	int mWidth, mHeight; //All the frame textures ahs this size (can be change)
	bool depthTexture; //True if  we have a depth texture
	bool deferredFBO; //True if this frameBufer has being created for deferred
	bool quadSet; //Control the creation of the vertex buffers for the screen quad
	GLuint vbo, vao; //For the quad

	//Holds the textures Id's
	std::vector<GLuint> mTexturesIdVector;

	//SceneManager pointer
	SceneManager* mCurrentSceneManager;
};

#endif