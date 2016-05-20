#version 400

in vec4 vPosition;
in vec2 vTexcoord;

out vec4 position;
out vec2 texcoord;
out vec4 shadowcoord;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

uniform mat4 depth_model_matrix;
uniform mat4 depth_view_matrix;
uniform mat4 depth_projection_matrix;

vec4 GetShadowCoord()
{
	mat4 biasMatrix = mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	mat4 depthMVP = depth_projection_matrix * depth_view_matrix * depth_model_matrix;
	vec4 depthposition = depthMVP * vPosition;
	depthposition.a = 1;
	vec4 shadowcoord = biasMatrix * depthposition;
	return shadowcoord;
}

void main()
{
	position = vPosition;
	texcoord = vTexcoord;
	shadowcoord = GetShadowCoord();
	gl_Position = vec4(texcoord * 2 - 1, 1, 1);
}