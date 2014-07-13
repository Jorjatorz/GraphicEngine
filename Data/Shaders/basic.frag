#version 430 core

layout(location = 0) out vec4 outColor; //layout(location = 0)
layout(location = 1) out vec4 outNormals; //layout(location = 1)

in vec3 vNormal;
in vec3 vPosition;
in vec2 uv;

uniform sampler2D baseTexture;
uniform vec3 baseColorVector;
uniform int isTextured;

void main()
{
	outNormals = vec4(vNormal, 1.0);
		
	if(isTextured == 1)
	{
		outColor = texture(baseTexture, uv);
	}
	else
	{
		outColor = vec4(baseColorVector, 1.0);
	}
}
