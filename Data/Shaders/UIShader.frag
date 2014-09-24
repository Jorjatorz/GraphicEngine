#version 430 core

layout(location = 0) out vec4 outColor; //layout(location = 0)

in vec2 uv;

uniform sampler2D UITexture;

void main()
{
	outColor = texture(UITexture, uv);
}
