#version 400
in vec2 texcoord;

out vec4 fColor;

uniform sampler2D map_bump;
uniform sampler2D map_toblur;

uniform int blur_type;
//blur type == 1, v_blur
//blur type == 2, h_blur
uniform int gaussion_type;

uniform float mode;

void main()
{
	float gaussion_para[75];
	int offset = gaussion_type * 15;
	//gaussion_type = 0, sigma = 0.036
	gaussion_para[0] = 0;
	gaussion_para[1] = 0;
	gaussion_para[2] = 0;
	gaussion_para[3] = 0;
	gaussion_para[4] = 0;
	gaussion_para[5] = 0;
	gaussion_para[6] = 0;
	gaussion_para[7] = 1;
	gaussion_para[8] = 0;
	gaussion_para[9] = 0;
	gaussion_para[10] = 0;
	gaussion_para[11] = 0;
	gaussion_para[12] = 0;
	gaussion_para[13] = 0;
	gaussion_para[14] = 0;

	//gaussion_type = 1, sigma = 0.14
	gaussion_para[1 * 15 + 0] = 0;
	gaussion_para[1 * 15 + 1] = 0;
	gaussion_para[1 * 15 + 2] = 0;
	gaussion_para[1 * 15 + 3] = 0;
	gaussion_para[1 * 15 + 4] = 0;
	gaussion_para[1 * 15 + 5] = 0;
	gaussion_para[1 * 15 + 6] = 0.000178;
	gaussion_para[1 * 15 + 7] = 0.999645;
	gaussion_para[1 * 15 + 8] = 0.000178;
	gaussion_para[1 * 15 + 9] = 0;
	gaussion_para[1 * 15 + 10] = 0;
	gaussion_para[1 * 15 + 11] = 0;
	gaussion_para[1 * 15 + 12] = 0;
	gaussion_para[1 * 15 + 13] = 0;
	gaussion_para[1 * 15 + 14] = 0;

	//gaussion_type = 2, sigma = 0.91
	gaussion_para[2 * 15 + 0] = 0;
	gaussion_para[2 * 15 + 1] = 0;
	gaussion_para[2 * 15 + 2] = 0;
	gaussion_para[2 * 15 + 3] = 0.00006;
	gaussion_para[2 * 15 + 4] = 0.002945;
	gaussion_para[2 * 15 + 5] = 0.046635;
	gaussion_para[2 * 15 + 6] = 0.241708;
	gaussion_para[2 * 15 + 7] = 0.417304;
	gaussion_para[2 * 15 + 8] = 0.241708;
	gaussion_para[2 * 15 + 9] = 0.046635;
	gaussion_para[2 * 15 + 10] = 0.002945;
	gaussion_para[2 * 15 + 11] = 0.00006;
	gaussion_para[2 * 15 + 12] = 0;
	gaussion_para[2 * 15 + 13] = 0;
	gaussion_para[2 * 15 + 14] = 0;

	//gaussion_type = 3, sigma = 7.0
	gaussion_para[3 * 15 + 0] = 0.048277;
	gaussion_para[3 * 15 + 1] = 0.055112;
	gaussion_para[3 * 15 + 2] = 0.061647;
	gaussion_para[3 * 15 + 3] = 0.067566;
	gaussion_para[3 * 15 + 4] = 0.07256;
	gaussion_para[3 * 15 + 5] = 0.076352;
	gaussion_para[3 * 15 + 6] = 0.078721;
	gaussion_para[3 * 15 + 7] = 0.079527;
	gaussion_para[3 * 15 + 8] = 0.078721;
	gaussion_para[3 * 15 + 9] = 0.076352;
	gaussion_para[3 * 15 + 10] = 0.07256;
	gaussion_para[3 * 15 + 11] = 0.067566;
	gaussion_para[3 * 15 + 12] = 0.061647;
	gaussion_para[3 * 15 + 13] = 0.055112;
	gaussion_para[3 * 15 + 14] = 0.048277;

	gaussion_para[4 * 15 + 0] = 0.000489;
	gaussion_para[4 * 15 + 1] = 0.002403;
	gaussion_para[4 * 15 + 2] = 0.009246;
	gaussion_para[4 * 15 + 3] = 0.027840;
	gaussion_para[4 * 15 + 4] = 0.065602;
	gaussion_para[4 * 15 + 5] = 0.120999;
	gaussion_para[4 * 15 + 6] = 0.174697;
	gaussion_para[4 * 15 + 7] = 0.197448;
	gaussion_para[4 * 15 + 8] = 0.174697;
	gaussion_para[4 * 15 + 9] = 0.120999;
	gaussion_para[4 * 15 + 10] = 0.065602;
	gaussion_para[4 * 15 + 11] = 0.027840;
	gaussion_para[4 * 15 + 12] = 0.009246;
	gaussion_para[4 * 15 + 13] = 0.002403;
	gaussion_para[4 * 15 + 14] = 0.000489;

	float bias = 1.0 / 800;
	vec2 tex_base = vec2(0,0);
	if(blur_type == 1) tex_base = vec2(0, 1);
	if(blur_type == 2) tex_base = vec2(1, 0);

	vec2 tex[15];
	for(int i = 0; i < 15; i ++)
		tex[i] = texcoord + tex_base * bias * (i - 7);
	
	vec4 blur_color = vec4(0, 0, 0, 0);
	bool is_margin = false;
	vec4 max_neighbour_color = vec4(0,0,0,1);
	for(int i = 0; i < 15; i ++){
		vec4 neighbour_color = texture2D(map_toblur, tex[i]);
		if(neighbour_color.x > max_neighbour_color.x) max_neighbour_color = neighbour_color;
		blur_color = blur_color + neighbour_color * gaussion_para[gaussion_type * 15 + i];
		if(neighbour_color == vec4(0,0,0,1)) is_margin = true;
	}

	if(is_margin) blur_color = texture2D(map_toblur, tex[7]);
	//if(mode == 6) blur_color = texture2D(map_toblur, tex[7]);
	blur_color.a = 1;
	fColor = blur_color;
}