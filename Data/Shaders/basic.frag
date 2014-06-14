#version 430 core

out vec4 outColor; //layout(location = 0)
out vec4 outPosition; //layout(location = 1)
out vec4 outNormals; //layout(location = 2)

in vec3 vNormal;
in vec3 vPosition;
in vec2 uv;

uniform sampler2D baseTexture;
uniform float mTime;
uniform vec3 baseColorVector;

void main()
{	
	vec3 n = normalize(vNormal);
	
	vec3 light = vec3(0.0, 0.0, 1.0);
    vec3 lightDir = light - vPosition.xyz ;;
    lightDir = normalize(lightDir);
    
    vec3 eyeDir = normalize(-vPosition.xyz);
    vec3 vHalfVector = normalize(lightDir + eyeDir);

	outPosition = vec4(vPosition, 1.0);
	outNormals = vec4(vNormal, 1.0);
	
	outColor = texture(baseTexture, uv *4);
}
