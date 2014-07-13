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
	vec3 pos  = persPos.xyz /persPos.w;
	vec2 uv =  pos.xy *0.5+0.5 ;



	float depth  = texture(depthTex, uv).x *2.0-1.0 ;
	vec3 N   =texture(normalTex, uv).xyz *2.0-1.0 ;


	pos.z =depth;
	vec4 worldPosSurf = inverseProjView * vec4(pos,1.0);
	worldPosSurf.xyz /= worldPosSurf.w;
	


	vec3 dir = worldPosSurf.xyz- lPos;

	float l = length( dir);
	vec3 viewDir = normalize(cameraPos - pos);
	vec3 halfDir = normalize(l + viewDir);
	
		float rFactor = clamp(dot(halfDir, N), 0.0, 1.0);
	float sFactor = pow(rFactor, 33.0);
	
	float dist =1.0-pow((clamp (l ,0.0,1.0)/1.0),2.0);


	float lambert  =clamp(dot(normalize(dir) , -N ),0.0,1.0);



   outDiff = vec4(lightColor *dist * lambert   ,1.0);
	
  	outSpecular = vec4(lightColor * sFactor * dist, 1.0);;
	
}
