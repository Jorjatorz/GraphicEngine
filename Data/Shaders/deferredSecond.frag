#version 430 core

out vec4 outColor;           // layout(location = 0)

uniform sampler2D posTex;
uniform sampler2D normalTex;
uniform sampler2D diffuseTex;

in vec2 uv;

void main(void)
{
   // Load data, stored in textures, from the first stage rendering.
   vec4 diffuse = texture(diffuseTex, uv);
   vec4 worldPos = texture(posTex, uv);
   vec4 normal = texture(normalTex, uv);

   outColor = vec4(1.0, 1.0, 1.0, 1.0) - diffuse;

}