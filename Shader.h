#ifndef SHADER_H
#define SHADER_H

#include "Definitions.h"

#include <string>

class Shader
{
public:
	Shader(std::string shaderName);
	~Shader(void);

	//shader locations
	typedef enum 
	{
		VERTEXPOSITION, //0
		VERTEXNORMALS, //1
		VERTEXTEXCOORD //2
	} shaderLocations;

	void loadShader(std::string filePath);

	GLuint getShaderProgram();

	// Communication with shader
	void UniformTexture(const std::string& uniformName, GLint activeTextureSlot);
	void Uniform(const std::string& uniformName, GLint value);
	void Uniform(const std::string& uniformName, GLfloat value);
	void Uniform(const std::string& uniformName, const glm::vec2& value);
	void Uniform(const std::string& uniformName, const glm::vec3& value);
	void Uniform(const std::string& uniformName, const glm::vec4& value);
	void UniformMatrix(const std::string& uniformName, const glm::mat4& value);
	void UniformMatrix(const std::string& uniformName, const glm::mat3& value);
	//void UniformMatrix(const std::string& uniformName, const aiMatrix4x4& value);
	void UniformTuniformNameure(const std::string& uniformName, GLint slot);
	void UniformBlock(const std::string& ext, GLuint location);

private:
	GLuint mProgram;
	std::string mName;

	void bind(); //bind the shader to use (gl_UsePorgram)
	void unBind(); //unbind the shader

	friend class SceneManager;
};

#endif