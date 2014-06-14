#include "Texture.h"


Texture::Texture(void)
{
	mWidth = mHeight = 0;
	mFormat = -1;
	isMipmap = false;
}


Texture::~Texture(void)
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::loadTexture(std::string filePath, bool mipmap, GLint format)
{
	SDL_Surface* image = IMG_Load(filePath.c_str());

	if (!image)
	{
		std::cout << "Error: " << IMG_GetError() << std::endl;
		mTextureID = -1;
	}
	else
	{
		generateTexture(image->w, image->h, format, mipmap, image->pixels);

		std::cout << "Texture loaded: " << filePath << std::endl;

		SDL_FreeSurface(image);
	}
}

void Texture::generateTexture(int width, int height, GLint format, bool mipmap, const GLvoid* pixels)
{
	GLuint tex;
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		isMipmap = true;
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		isMipmap = false;
	}

	mFormat = format;
	mWidth = width;
	mHeight = height;

	mTextureID = tex;
}