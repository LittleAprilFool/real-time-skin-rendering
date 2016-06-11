#version 400

in vec4 position;
in vec2 texcoord;

uniform float mode;

uniform sampler2D map_kd;
uniform sampler2D map_bump;
uniform sampler2D map_depth;
uniform sampler2D map_scattered;
uniform sampler2D map_blur;
uniform sampler2D map_light;
uniform sampler2D map_shadow;
uniform sampler2D map_thickness;


layout(location = 0) out vec4 fColor;

void main()
{
	if(mode == 7) fColor = texture(map_kd, texcoord);
	if(mode == 6) fColor = texture(map_bump, texcoord);
	if(mode == 5) fColor = texture(map_depth, texcoord);
	if(mode == 4) fColor = texture(map_scattered, texcoord);
	if(mode == 3) fColor = texture(map_blur, texcoord);
	if(mode == 2) fColor = texture(map_light, texcoord);
	if(mode == 1) fColor = texture(map_thickness, texcoord);
	fColor.a = 1;
}