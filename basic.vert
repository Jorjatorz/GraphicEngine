#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 finalM;

void main()
{
	gl_Position =  finalM * vec4(position, 1.0);
}