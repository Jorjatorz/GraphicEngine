
uniform mat4 PV;


void main()
{
gl_FrontColor = gl_Color;
	gl_Position = PV  * gl_Vertex;
}