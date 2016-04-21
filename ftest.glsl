#version 400

in vec2 texcoord;
out vec4 fColor;

uniform sampler2D map_kd;
uniform sampler2D map_bump;

void main()
{
	fColor = texture(map_kd, texcoord);
}