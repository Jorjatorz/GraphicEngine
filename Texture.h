#ifndef TEXTURE_H
#define TEXTURE_H

#include "Definitions.h"

#include <iostream>

#include <string>

class Texture
{
public:
	Texture(void);
	~Texture(void);

	void loadTexture(std::string filePath, bool mipmap, GLint format);
	void generateTexture(int width, int height, GLint format, bool mipmap, const GLvoid* pixels);
	void updateTexture(int width, int height, GLint format, bool mipmap, const GLvoid* pixels);


	//is mipmap
	bool isMipmap;
	//Texture format
	GLint mFormat;
	//Texture dimensions
	int mWidth, mHeight;
	//texture ID
	GLuint mTextureID;


};

#endif