#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 vNormal;
out vec2 uv;

uniform mat4 MVP; //ModelViewPerspective matrix
uniform mat4 modelM;
uniform mat4 viewM;
uniform mat4 perspectiveM;
uniform mat4 normalM;

void main()
{
	uv = texCoord;	
	vNormal = (normalM * vec4(normal, 1.0)).xyz * 0.5 + 0.5;
	
	gl_Position =  MVP * vec4(position, 1.0);
}