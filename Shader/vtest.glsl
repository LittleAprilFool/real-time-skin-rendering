#version 400

in vec4 vposition;
in vec2 vtexcoord;

out vec4 position;
out vec2 texcoord;

void main()
{

	//compute position
	position = vposition;
	gl_Position = vec4((vtexcoord - 0.5) * 2, 1,1);
	texcoord = vtexcoord;
}