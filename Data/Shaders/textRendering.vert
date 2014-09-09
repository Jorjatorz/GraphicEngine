#version 430 core
layout (location = 0) in vec4 coords;
out vec2 uv;
 
void main(void) 
{
  gl_Position = vec4(coords.xy, -0.1, 1); // Z = -0.1 so its in front of the UI
  uv = coords.zw;
}