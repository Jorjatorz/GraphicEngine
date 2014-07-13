#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 MVP; //ModelViewPerspective matrix
uniform mat4 modelM;
uniform mat4 viewM;
uniform mat4 projectionM;
uniform mat4 normalM;
uniform vec3 lightPos;

out mat4 inverseProjView;
out vec4 persPos;
out vec3 lPos;

void main()
{
	gl_Position =  MVP * vec4(position, 1.0);
	persPos = gl_Position;
	
	lPos = (viewM * modelM * vec4(lightPos, 1.0)).xyz;
	
	inverseProjView = inverse(projectionM);
}