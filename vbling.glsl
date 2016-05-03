#version 400

in vec4 vposition;
in vec2 vtexcoord;
in vec4 vnormal;
in vec4 vtangent;

out vec4 position;
out vec2 texcoord;
out vec4 normal;
out vec4 tangent;
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
	vec4 depthposition = depthMVP * vposition;
	depthposition.a = 1;
	vec4 shadowcoord = biasMatrix * depthposition;
	return shadowcoord;
}

void main()
{
	position = model_matrix * vposition;
	texcoord = vtexcoord;
	normal = model_matrix * vnormal;
	tangent = model_matrix * vtangent;
	shadowcoord = GetShadowCoord();
	//compute position
	gl_Position = projection_matrix * view_matrix * model_matrix * vposition;
	gl_Position.a = 1;
}