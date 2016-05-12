#version 400

in vec4 vPosition;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
  //compute position
  //gl_Position =projection_matrix * view_matrix * model_matrix * vPosition;
	vec4 position = view_matrix * model_matrix * vPosition;
	position.a = 1;
	gl_Position = position;
}