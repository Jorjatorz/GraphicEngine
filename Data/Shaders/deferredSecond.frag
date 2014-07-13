#version 430 core

out vec4 outColor;           // layout(location = 0)

uniform sampler2D lightTex;
uniform sampler2D specTex;
uniform sampler2D diffTex;

in vec2 uv;

void main(void)
{
   // Load data, stored in textures, from the first stage rendering.
  
  vec4 diffuse = texture(diffTex, uv);
  vec4 light = texture(lightTex, uv);
  vec4 specular = texture(specTex, uv);

   outColor = diffuse * 0.2;
   outColor += diffuse * light;
   outColor += specular;
   outColor.a = 1.0;

}