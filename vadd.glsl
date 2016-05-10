#version 400

in vec4 vposition;
in vec2 vtexcoord;
in vec4 vnormal;
in vec4 vtangent;

out vec2 texcoord;

void main()
{
	texcoord = vtexcoord;
	texcoord = texcoord * 2 - 1;
	gl_Position = vec4(vtexcoord, 1, 1);
	texcoord = vtexcoord;
}