#ifndef SHADER_H
#define SHADER_H

#include "Definitions.h"

#include <string>

class Shader
{
public:
	Shader(void);
	~Shader(void);

	void loadShader(std::string filePath);

	GLuint getShaderProgram();

private:
	GLuint mProgram;

	void bind(); //bind the shader to use (gl_UsePorgram)
	void unBind(); //unbind the shader


	friend class SceneManager;
};

#endif