#version 150

out vec4 outColor;

in vec3 mNorm;

uniform float mTime;

void main()
{	
	outColor = vec4(mNorm, 1.0) * sin(mTime * 0.001);
}
