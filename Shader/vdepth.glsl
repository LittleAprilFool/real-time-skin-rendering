#version 400

in vec4 vPosition;
in vec2 vTexcoord;

uniform mat4 depth_model_matrix;
uniform mat4 depth_view_matrix;
uniform mat4 depth_projection_matrix;

out vec4 position;
out vec2 texcoord;
void main()
{
	position = vPosition;
	texcoord = vTexcoord;
	mat4 depth_mvp_matrix = depth_projection_matrix * depth_view_matrix * depth_model_matrix;
	gl_Position = depth_mvp_matrix * vPosition;
	gl_Position.a = 1;
	//gl_Position = vec4(texcoord * 2 - 1, 1, 1);
}