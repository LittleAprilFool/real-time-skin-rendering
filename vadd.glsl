#version 400

in vec4 vposition;
in vec2 vtexcoord;
in vec4 vnormal;
in vec4 vtangent;

out vec4 position;
out vec2 texcoord;

void main()
{
	position = vposition;
	texcoord = vtexcoord;
	gl_Position = vec4(texcoord * 2 - 1, 1, 1);
}