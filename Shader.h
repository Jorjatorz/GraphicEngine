#ifndef SHADER_H
#define SHADER_H

#include "Definitions.h"

#include <string>

class Shader
{
public:
	Shader(void);
	~Shader(void);

	//shader locations
	typedef enum 
	{
		vertexPosition, //0
		vertexNormal, //1
		vertexTexture //2
	} shaderLocations;

	void loadShader(std::string filePath);

	GLuint getShaderProgram();

private:
	GLuint mProgram;

	void bind(); //bind the shader to use (gl_UsePorgram)
	void unBind(); //unbind the shader

	friend class SceneManager;
};

#endif