#version 150

out vec4 outColor;

in vec3 mNorm;
in vec3 vPosition;

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

	outColor = vec4(max(dot(n.xyz,lightDir),0) * baseColorVector.xyz + pow(max(dot(n.xyz,vHalfVector),0.0), 100) * 1.5, 1.0);
}
