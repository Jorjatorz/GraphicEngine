#version 430 core

layout (location = 0) out vec4 outDiff; //layout(location = 0)
layout (location = 1) out vec4 outSpecular; //layout(location = 1)

uniform sampler2D depthTex;
uniform sampler2D normalTex;
uniform sampler2D diffuseTex;

uniform vec3 cameraPos;

uniform vec3 lightColor;
uniform float lightRadius;
uniform float lightCutOff;
uniform int lightType;
uniform vec3 lightDirection;


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
	
	//Pointlight
	if(lightType == 0)
	{
		//We check if the fragment is inside the lighVolume, if its not Discard.
		float dist = length(lPos - pos);
		float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
		
		if(atten == 0.0)
		{
		discard;
		}
		
	
		vec4 spec = vec4(0.0);
	
		vec3 eye = normalize(cameraPos - pos);
		vec3 l = normalize(lPos - pos);		
		
		float intensity = max(dot(normal, l), 0.0);
		
		if(intensity > 0.0)
		{
			vec3 h = normalize(l + eye);
			
			float intSpec = max(dot(h, normal), 0.0);
			spec = vec4(lightColor * pow(intSpec, 33.0), 1.0);
		}
		
		outDiff = vec4(lightColor * intensity * atten, 1.0);
		outSpecular = spec * atten;
	}
	else if(lightType == 1) //Directional
	{
			vec4 spec = vec4(0.0);
	
			vec3 eye = normalize(cameraPos - pos);
			
			float intensity = max(dot(normal, -lightDirection), 0.0);
			
			if(intensity > 0.0)
			{
				vec3 h = normalize(lightDirection + eye);
				
				float intSpec = max(dot(h, normal), 0.0);
				spec = vec4(lightColor * pow(intSpec, 33.0), 1.0);
			}
			
			outDiff = vec4(lightColor * intensity, 1.0);
			outSpecular = spec;
	}
	else if(lightType == 2)
	{
		//We check if the fragment is inside the lighVolume, if its not Discard.
		float dist = length(lPos - pos);
		float atten = 1.0 /(1.0 + 0.1*dist + 0.01*dist*dist);
		
		if(atten == 0.0)
		{
		discard;
		}
	
		float intensity = 0.0;
		vec4 spec = vec4(0.0);
	 
		vec3 L = normalize(lPos - pos);
		vec3 D = normalize(lightDirection);  
	 
	 
		// inside the cone?
		if (dot(-L, D) > 0.91f) {
	 
			intensity = max(dot(normal,L), 0.0);
	 
			if (intensity > 0.0) {
				vec3 eye = normalize(cameraPos - pos);
				vec3 R = reflect(-L, normal);
				
				float specular = pow(max(dot(R, eye), 0.0), 33.0);
				
				spec = vec4(lightColor * specular, 1.0);
			}
		}
	 
		outDiff = vec4(lightColor * intensity * atten, 1.0);
		outSpecular = spec * atten;
	}
	
	
}
