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
		addTexture(GL_RGBA);
		//Position
		addTexture(GL_RGBA32F);
		//Normal
		addTexture(GL_RGBA16);
		//Depth and stencil
		glGenRenderbuffers(1, &mRenderBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight); //24 bits depth 8 stencil
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferId);

		//check the fbo
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			printf("FB error, status: 0x%x\n", Status);
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		deferredFBO = true;
	}
}

void FrameBuffer::bindForDrawing()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
	GLuint windowBuffClear[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2}; //We have 4 color attachments (can be more, put at will)
	glDrawBuffers(mTexturesIdVector.size(), windowBuffClear); // Select all buffers
}

void FrameBuffer::bindForRendering()
{
	if (!quadSet)
	{
		//vertices
		const GLfloat vertex_positions[] =
		{
			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,

			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

		glVertexAttribPointer(Shader::VERTEXPOSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glVertexAttribPointer(Shader::VERTEXTEXCOORD, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		quadSet = true;
	}

	// Bind default framebuffer and draw contents of our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(vao);
	glDisable(GL_DEPTH_TEST);

	mCurrentSceneManager->bindShader(mCurrentSceneManager->createShader("deferredSecond", "deferredSecond"));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexturesIdVector.at(0));
	mCurrentSceneManager->getCurrentShader()->UniformTexture("diffuseTex", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTexturesIdVector.at(1));
	mCurrentSceneManager->getCurrentShader()->UniformTexture("posTex", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mTexturesIdVector.at(2));
	mCurrentSceneManager->getCurrentShader()->UniformTexture("normalTex",2);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::unBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}