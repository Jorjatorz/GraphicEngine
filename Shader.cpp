#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(std::string shaderName)
{
	mProgram = 0;
	mName = shaderName;
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

	glBindFragDataLocation(program, 0, "outColor"); //output color to the buffer number 0

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

// sent texture value
void Shader::UniformTexture(const std::string& uniformName, GLint textureId)
{
	//glActiveTexture(GL_TEXTURE0+slot);
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif
	glUniform1i(id, textureId);
}

// sent int value
void Shader::Uniform(const std::string& uniformName, GLint value)
{
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif
	glUniform1iARB(id, value);
}

// sent float value
void Shader::Uniform(const std::string& uniformName, GLfloat value)
{
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif
	glUniform1fARB(id, value);
}

// sent vec2 value
void Shader::Uniform(const std::string& uniformName, const glm::vec2& value)
{
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif
	glUniform2fvARB(id, 1, glm::value_ptr(value));
}


// Le shader doit être activé avant
void Shader::Uniform(const std::string& uniformName, const glm::vec3& value)
{
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif
	glUniform3fvARB(id, 1, glm::value_ptr(value));
}

void Shader::Uniform(const std::string& uniformName, const glm::vec4& value)
{
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif
	glUniform3fvARB(id, 1, glm::value_ptr(value));
}

void Shader::UniformMatrix(const std::string& uniformName, const glm::mat4& value)
{
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif
	glUniformMatrix4fvARB(id, 1, false, glm::value_ptr(value));
}

void Shader::UniformMatrix(const std::string& uniformName, const glm::mat3& value)
{
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif
	glUniformMatrix4fvARB(id, 1, false, glm::value_ptr(value));
}
/*
void Shader::UniformMatrix(const std::string& uniformName, const aiMatrix4x4& value)
{
	GLuint id = glGetUniformLocation(mProgram, uniformName.c_str());
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;

	glm::mat4 temp(1.0);

	for(unsigned int i = 0; i < 4; i++)
	{
		for(unsigned int j = 0; j < 4; j++)
		{
			temp[i][j] = value[i][j];
		}
	}
	glUniformMatrix4fvARB(id, 1, false, glm::value_ptr(temp));
}
*/

void Shader::UniformBlock(const std::string& uniformName, GLuint  location)
{
	GLuint id = glGetUniformBlockIndex(mProgram, uniformName.c_str());
	#ifdef SHADER_MESSAGES
	if(id == -1)
		std::cout << mName << " Couldn't get uniform location of " << uniformName << std::endl;
	#endif

	glUniformBlockBinding(mProgram, id, location);
}