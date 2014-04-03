#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(void)
{
	mProgram = 0;
}


Shader::~Shader(void)
{
	//delete the program
	glDeleteProgram(mProgram);
}

void Shader::loadShader(std::string filePath)
{
	std::ifstream file;
	std::stringstream strStreamBuffer; //string stream buffer

	//vertex-------------------------------------

	//load
	file.open(filePath + ".vert");

	std::string vertexShaderStr; //string

	if (!file.is_open())
	{
		std::cout << "Error while opening " << filePath + ".vert" << std::endl;
	}
	else
	{
		strStreamBuffer << file.rdbuf(); //read the file
		vertexShaderStr = strStreamBuffer.str(); //copy to a string

		file.close();
	}


	//create
	const GLchar* vertexShaderSource = vertexShaderStr.c_str(); //create the source

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check vertex shader
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);


	if (status == 0)
	{
		std::cout << "Vertex Shader" << std::endl;
		std::cout << buffer << std::endl;
	}

	//fragment------------------------

	//load
	file.open(filePath + ".frag");

	std::string fragmentShaderStr; //strings

	if (!file.is_open())
	{
		std::cout << "Error while opening " << filePath + ".frag" << std::endl;
	}
	else
	{
		//reset stream buffer
		strStreamBuffer.str("");
		strStreamBuffer.clear();

		strStreamBuffer << file.rdbuf();
		fragmentShaderStr = strStreamBuffer.str();

		file.close();
	}

	//create
	const GLchar* fragmentShaderSource = fragmentShaderStr.c_str();

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check fragment shader
	GLint status2;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status2);
	char buffer2[512];
	glGetShaderInfoLog(fragmentShader, 512, NULL, buffer2);

	if (status2 == 0)
	{
		std::cout << "Fragment Shader" << std::endl;
		std::cout << buffer2 << std::endl;
	}


	// Link the vertex and fragment shader into a shader program--------------------
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glBindFragDataLocation(program, 0, "outColor");
	glLinkProgram(program);
	
	//we finished so delete the sahder
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	//save the program
	mProgram = program;
}

GLuint Shader::getShaderProgram()
{
	return mProgram;
}

void Shader::bind()
{
	glUseProgram(mProgram); //binds the shader program
}
void Shader::unBind()
{
	glUseProgram(0); //unbids the shader program
}