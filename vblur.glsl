#version 400

in vec4 vposition;
in vec2 vtexcoord;
in vec4 vnormal;
in vec4 vtangent;

out vec4 position;
out vec4 normal;
out vec4 tangent;
out vec2 texcoord;

void main()
{
	texcoord = vtexcoord;
	texcoord = texcoord * 2 - 1;
	gl_Position = vec4(texcoord, 1, 1);
	position = vposition;
	normal = vnormal;
	tangent = vtangent;
	texcoord = vtexcoord;
}