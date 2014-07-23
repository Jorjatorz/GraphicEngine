#version 430 core

layout (location = 0) out vec4 outDiff; //layout(location = 0)
layout (location = 1) out vec4 outSpecular; //layout(location = 1)

uniform sampler2D depthTex;
uniform sampler2D normalTex;
uniform sampler2D diffuseTex;

uniform vec3 cameraPos;

uniform vec3 lightColor;


in mat4 inverseProjView;
in vec4 persPos;
in vec3 lPos;

void main()
{
	vec3 pos = vec3(gl_FragCoord.x / 1280, gl_FragCoord.y / 720, 0.0);
	
	pos.z = texture(depthTex, pos.xy).r;
	
	vec3 normal = normalize(texture(normalTex, pos.xy).xyz * 2.0 - 1.0);
	
	vec4 clip = inverseProjView * vec4(pos * 2.0 - 1.0, 1.0);
	pos = clip.xyz / clip.w;
	
	float dist = length(lPos - pos);
	float atten = 1.0 - clamp(dist / 1.0, 0.0, 1.0);
	
	if(atten == 0.0)
	{
	discard;
	}
	
	vec3 incident = normalize(lPos - pos);
	vec3 viewDir = normalize(cameraPos - pos);
	vec3 halfDir = normalize(incident + viewDir);
	
	float lambert = clamp(dot(incident, normal), 0.0, 1.0);
	float rFactor = clamp(dot(halfDir, normal), 0.0, 1.0);
	float sFactor = pow(rFactor, 33.0);
	
	outDiff = vec4(lightColor * lambert * atten, 1.0);
	outSpecular = vec4(lightColor * sFactor * atten * 0.33, 1.0);
	
	
}
