#version 400
in vec4 position;
in vec4 normal;
in vec4 tangent;
in vec2 texcoord;

out vec4 fColor;

uniform sampler2D map_bump;
uniform sampler2D map_toblur;

void main()
{
	vec4 weight[9];

	weight[0] = vec4(0.0625, 0.125,  0.125,  0.0);
	weight[1] = vec4(0.125, 0.0,  0.0,  0.0);
	weight[2] = vec4(0.0625, 0.0,  0.0,  0.0);
	weight[3] = vec4(0.125, 0.125,  0.125,  0.0);
	weight[4] = vec4(0.25, 0.5, 0.5, 0.0);
	weight[5] = vec4(0.125, 0.125,  0.125,  0.0);
	weight[6] = vec4(0.0625, 0.0,  0.0,  0.0);
    weight[7] = vec4(0.125, 0.125,  0.125,  0.0);
    weight[8] = vec4(0.0625, 0.0,  0.0,  0.0);

	vec2 tex[9];
	float bias = 0.005;
	tex[0] = texcoord + vec2(-bias, bias);
	tex[1] = texcoord + vec2(0, bias);
	tex[2] = texcoord + vec2(bias, bias);
	tex[3] = texcoord + vec2(-bias, 0);
	tex[4] = texcoord + vec2(0, 0);
	tex[5] = texcoord + vec2(bias, 0);
	tex[6] = texcoord + vec2(-bias, -bias);
	tex[7] = texcoord + vec2(0, -bias);
	tex[8] = texcoord + vec2(bias, -bias);
	
	vec4 blur_color = vec4(0, 0, 0, 0);
	bool is_margin = false;
	vec4 max_neighbour_color = vec4(0,0,0,1);
	for(int i = 0; i < 9; i ++){
		vec4 neighbour_color = texture2D(map_toblur, tex[i]);
		if(neighbour_color.x > max_neighbour_color.x) max_neighbour_color = neighbour_color;
		blur_color = blur_color + neighbour_color * weight[i];
		if(neighbour_color == vec4(0,0,0,1)) is_margin = true;
	}

	if(is_margin) blur_color = texture2D(map_toblur, tex[4]);
	blur_color.a = 1;
	fColor = blur_color;
}