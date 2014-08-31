#version 430 core

layout (location = 0) in vec2 position;
layout (location = 2) in vec2 texCoord;

out vec2 uv;


uniform mat4 modelM;

void main()
{
	uv = texCoord;
	
	gl_Position =  modelM * vec4(position, 0.0, 1.0);
}