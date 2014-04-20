#version 150

out vec4 outColor;

in vec3 mNorm;

void main()
{	
	outColor = vec4(mNorm, 1.0);
}
