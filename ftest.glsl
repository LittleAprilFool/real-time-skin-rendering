#version 400

in vec2 texcoord;
out vec4 fColor;

uniform sampler2D map_rendered;
uniform sampler2D map_light;

void main()
{
	fColor = texture2D(map_light, texcoord);
}