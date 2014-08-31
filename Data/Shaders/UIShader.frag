#version 430 core

layout(location = 0) out vec4 outColor; //layout(location = 0)

in vec2 uv;

uniform vec4 UIColor;
uniform bool isTextured;
uniform sampler2D UITexture;

void main()
{
	if(!isTextured)
	{
	outColor = UIColor;
	}
	else
	{
		outColor = texture(UITexture, uv);
	}
}
