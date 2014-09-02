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
	//FROM FREEIMAGE EXAMPLE
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL textureID
	mTextureID = -1;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filePath.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(filePath.c_str());
	}
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
	{
		std::cout << "Unknown format of texture, can't load it: " << filePath << std::endl;
		return;
	}

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, filePath.c_str());

		FREE_IMAGE_COLOR_TYPE type = FreeImage_GetColorType(dib);
		if (FREE_IMAGE_COLOR_TYPE::FIC_RGBALPHA != type)
		{
			dib = FreeImage_ConvertTo32Bits(dib);
		}
	}
	//if the image failed to load, return failure
	if (!dib)
	{
		std::cout << "Texture format not supported: " << filePath << std::endl;
		return;
	}

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
	{
		std::cout << "Error getting bits, width or height of texture: " << filePath << std::endl;
		return;
	}


	generateTexture(width, height, format, mipmap, bits);

	std::cout << "Texture loaded: " << filePath << std::endl;

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

}

void Texture::generateTexture(int width, int height, GLint format, bool mipmap, const GLvoid* pixels)
{
	GLuint tex;
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels); //Free iamge loads in BGR

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