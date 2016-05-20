#version 400
in vec2 texcoord;
layout(location = 0) out vec4 fColor;

uniform int gaussion_type;
uniform float toadd_weight;
uniform sampler2D map_toadd;
uniform sampler2D map_add;

void main()
{
	fColor = texture2D(map_toadd, texcoord) * toadd_weight + texture2D(map_add, texcoord);
}