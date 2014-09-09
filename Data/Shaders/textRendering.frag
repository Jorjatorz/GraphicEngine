#version 430 core

out vec4 outColor; 
 
in vec2 uv;

uniform sampler2D fontBitmap;
uniform vec3 fontColor;

 
void main(void) 
{
  outColor = vec4(fontColor, texture(fontBitmap, uv).a);
}