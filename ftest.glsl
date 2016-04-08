#version 400

in vec2 texcoord;
out vec4 fColor;

uniform sampler2D map_rendered;

void main()
{
	fColor = texture2D(map_rendered, texcoord);
}