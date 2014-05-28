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

	void loadTexture(std::string filePath, bool mipmap);


	//is mipmap
	bool isMipmap;
	//texture ID
	GLuint mTextureID;


};

#endif