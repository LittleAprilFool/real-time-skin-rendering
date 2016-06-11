#version 400

in vec4 vposition;
in vec2 vtexcoord;
out vec2 texcoord;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;


void main()
{
	texcoord = vtexcoord;
	//compute position
	gl_Position = projection_matrix * view_matrix * model_matrix * vposition;
	gl_Position.a = 1;
}