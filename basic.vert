#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 mNorm;

uniform mat4 finalM;
uniform mat4 normalM;

void main()
{
	mNorm = normalize((normalM * vec4(normal, 1.0)).xyz);
	gl_Position =  finalM * vec4(position, 1.0);
}