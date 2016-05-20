#version 400

in vec2 texcoord;
out vec4 fColor;


uniform float mode;
uniform sampler2D map_kd;
uniform sampler2D map_bump;
uniform sampler2D map_light;
uniform sampler2D map_shadow;
uniform sampler2D map_thickness;

void main()
{

	if(mode == 1) fColor = texture(map_light, texcoord);
	if(mode == 2) fColor = texture(map_shadow, texcoord);
	if(mode == 3) fColor = texture(map_thickness, texcoord);
	if(mode == 4) fColor = texture(map_kd, texcoord) * texture(map_light, texcoord);
	if(mode == 5) fColor = texture(map_kd, texcoord) * texture(map_light, texcoord) * texture(map_shadow, texcoord).x;
	if(mode == 6) fColor = texture(map_kd, texcoord) * texture(map_light, texcoord) * texture(map_shadow, texcoord).x + texture(map_thickness, texcoord) * 0.1;
	fColor.a = 1;
}