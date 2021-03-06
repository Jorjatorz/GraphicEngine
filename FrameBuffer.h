#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Definitions.h"

#include <string>
#include <vector>

class FrameBuffer
{
public:
	FrameBuffer(std::string name, int width, int height, SceneManager* manager);
	~FrameBuffer();

	void addTexture(int format);
	void createGBuffer();
	void bindForDrawing();
	void bindForReading(int idStart = 0); //Idstart is the start of the active texture (i.e if u bind several buffers at the same time this prevents the active textures to use the same location)
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

	//Holds the textures Id's
	std::vector<GLuint> mTexturesIdVector;

	//SceneManager pointer
	SceneManager* mCurrentSceneManager;
};

#endif