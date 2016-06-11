#version 400

in vec4 position;
in vec2 texcoord;
in vec4 normal;
in vec4 tangent;
in vec4 shadowcoord;

layout(location = 0) out vec4 fColor;

uniform float mode;

uniform sampler2D map_kd;
uniform sampler2D map_bump;
uniform sampler2D map_depth;
uniform sampler2D map_scattered;
uniform sampler2D map_blur;
uniform sampler2D map_light;
uniform sampler2D map_shadow;
uniform sampler2D map_thickness;


void main()
{
	vec4 kd = texture(map_kd, texcoord);
	
	vec4 light = texture(map_light, texcoord);
	
	vec4 shadow = texture(map_shadow, texcoord);

	vec4 thickness = texture(map_thickness, texcoord);

	vec2 bias = vec2(0.01, 0.01);

	if(light.x == 0) light = texture(map_light, texcoord - bias);
	if(light.x == 0) light = texture(map_light, texcoord + bias);

	if(shadow.x == 0) shadow = texture(map_shadow, texcoord - bias);
	if(shadow.x == 0) shadow = texture(map_shadow, texcoord + bias);

	bias = vec2(0.01, -0.01);

	if(light.x == 0) light = texture(map_light, texcoord - bias);
	if(light.x == 0) light = texture(map_light, texcoord + bias);

	if(shadow.x == 0) shadow = texture(map_shadow, texcoord - bias);
	if(shadow.x == 0) shadow = texture(map_shadow, texcoord + bias);

	vec4 light_intensity = vec4(1,0,0,1);

	if (mode == 7) light_intensity = kd;
	if (mode == 6) light_intensity = light;
	if (mode == 5) light_intensity = shadow;
	if (mode == 4) light_intensity = thickness;
	if (mode == 3) light_intensity = kd * light;
	if (mode == 2) light_intensity = kd * light * shadow;
	if (mode == 1) light_intensity = kd * light * shadow + thickness * 0.1; 
	fColor = vec4(light_intensity.xyz, 1);
}