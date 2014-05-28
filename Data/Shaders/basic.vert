#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 mNorm;
out vec3 vPosition;
out vec2 uv;

uniform mat4 finalM;
uniform mat4 normalM;

void main()
{
	vPosition = position;
	uv = texCoord;
	
	mNorm = vec3(normalM * vec4(normal, 1.0));
	gl_Position =  finalM * vec4(position, 1.0);
}