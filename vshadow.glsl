#version 400

in vec4 vPosition;

uniform mat4 depthMVP;
uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 mvp_matrix;
uniform mat4 depth_model_matrix;
uniform mat4 depth_view_matrix;
uniform mat4 depth_projection_matrix;

void main()
{
	mat4 depth_mvp_matrix = depth_projection_matrix * depth_view_matrix * depth_model_matrix;
	gl_Position = depth_mvp_matrix * vPosition;
}