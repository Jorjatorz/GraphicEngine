#version 430 core

out vec4 outColor;

in vec3 mNorm;
in vec3 vPosition;
in vec2 uv;

uniform sampler2D baseTexture;
uniform float mTime;
uniform vec3 baseColorVector;

void main()
{	
	vec3 n = normalize(mNorm);
	
	vec3 light = vec3(0.0, 0.0, 1.0);
    vec3 lightDir = light - vPosition.xyz ;;
    lightDir = normalize(lightDir);
    
    vec3 eyeDir = normalize(-vPosition.xyz);
    vec3 vHalfVector = normalize(lightDir + eyeDir);

	outColor = texture(baseTexture, uv *4);
}
