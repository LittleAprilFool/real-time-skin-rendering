#version 400

in vec4 vposition;
in vec2 vtexcoord;
in vec4 vnormal;
in vec4 vtangent;

out vec4 position;
out vec2 texcoord;
out vec4 normal;
out vec4 tangent;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
	position = vposition;
	texcoord = vtexcoord;
	normal = vnormal;
	tangent = vtangent;

	//compute position
	gl_Position = projection_matrix * view_matrix * model_matrix * position;
}