#include "FrameBuffer.h"

#include "Shader.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Texture.h"

FrameBuffer::FrameBuffer()
{
	mName = "Unnamed FrameBuffer";
	deferredFBO = false;
	quadSet = false;
	depthTexture = false;
	//Create it in the constructor
	glGenFramebuffers(1, &mFrameBufferId);
}

FrameBuffer::FrameBuffer(std::string name, int width, int height, SceneManager* manager)
{
	mName = name;
	mWidth = width;
	mHeight = height;
	deferredFBO = false;
	quadSet = false;

	mCurrentSceneManager = manager;
	//Create it in the constructor
	glGenFramebuffers(1, &mFrameBufferId);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteRenderbuffers(1, &mRenderBufferId);
	glDeleteFramebuffers(1, &mFrameBufferId);
	//Delete all the textures
	glDeleteTextures(mTexturesIdVector.size(), &mTexturesIdVector.at(0));
}

void FrameBuffer::addTexture(int format)
{
	GLuint id = mCurrentSceneManager->createTexture(mName + "Texture" + std::to_string(mTexturesIdVector.size()), false, format, "NULL", mWidth, mHeight)->mTextureID;
	
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + mTexturesIdVector.size(), GL_TEXTURE_2D, id, 0);

	mTexturesIdVector.push_back(id);
}

void FrameBuffer::createGBuffer()
{
	if (!deferredFBO)
	{
		//Color
		addTexture(GL_RGBA8);
		//Normal
		addTexture(GL_RGBA8);
		//Depth
		GLuint depthTex;
		glGenTextures(1, &depthTex);
		glBindTexture(GL_TEXTURE_2D, depthTex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		mTexturesIdVector.push_back(depthTex);

		depthTexture = true;

		//check the fbo
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			printf("FB error, status: 0x%x\n", Status);
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		deferredFBO = true;
	}
}

void FrameBuffer::bindForDrawing()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
	GLuint windowBuffClear[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	if (depthTexture)
	{
		glDrawBuffers(mTexturesIdVector.size() - 1, windowBuffClear); // Select all buffers except the depthBuffer
	}
	else
	{
		glDrawBuffers(mTexturesIdVector.size(), windowBuffClear);
	}
}

void FrameBuffer::bindForReading(int idStart)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//We suppose we have 3 textures

	for (int i = 0; i < mTexturesIdVector.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i + idStart);
		glBindTexture(GL_TEXTURE_2D, mTexturesIdVector.at(i));
	}
}

void FrameBuffer::bindForRendering()
{
	glBindVertexArray(ResourceManager::getSingletonPtr()->getScreenQuadVAO());
	glDisable(GL_DEPTH_TEST);

	mCurrentSceneManager->bindShader(mCurrentSceneManager->createShader("deferredSecond", "deferredSecond"));

	mCurrentSceneManager->getFrameBuffer("deferredFBO")->bindForReading();
	mCurrentSceneManager->getCurrentShader()->Uniform("diffTex", 0);
	mCurrentSceneManager->getFrameBuffer("lightFBO")->bindForReading(1);
	mCurrentSceneManager->getCurrentShader()->UniformTexture("lightTex", 1);
	mCurrentSceneManager->getCurrentShader()->UniformTexture("specTex", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mTexturesIdVector.at(0));
	mCurrentSceneManager->getCurrentShader()->UniformTexture("uiTex", 3);

	/*glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mTexturesIdVector.at(2));
	mCurrentSceneManager->getCurrentShader()->UniformTexture("normalTex",2);*/
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::unBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}