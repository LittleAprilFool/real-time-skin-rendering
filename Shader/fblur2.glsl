#version 400
in vec2 texcoord;
out vec4 fColor;

uniform sampler2D map_bump;
uniform sampler2D map_toblur;

uniform int blur_type;

uniform int gaussion_type;

uniform float mode;

void main()
{
	float gaussion_para[6 * 15];
	int offset = gaussion_type * 15;

	//gaussion_para[0 * 15 + ] =0;
	//gaussion_type = 0, variance = 0.0064
	gaussion_para[0 * 15 + 0] = 0;
	gaussion_para[0 * 15 + 1] = 0;
	gaussion_para[0 * 15 + 2] = 0;
	gaussion_para[0 * 15 + 3] = 0;
	gaussion_para[0 * 15 + 4] = 0;
	gaussion_para[0 * 15 + 5] = 0;
	gaussion_para[0 * 15 + 6] = 0;
	gaussion_para[0 * 15 + 7] = 1;
	gaussion_para[0 * 15 + 8] = 0;
	gaussion_para[0 * 15 + 9] = 0;
	gaussion_para[0 * 15 + 10] = 0;
	gaussion_para[0 * 15 + 11] = 0;
	gaussion_para[0 * 15 + 12] = 0;
	gaussion_para[0 * 15 + 13] = 0;
	gaussion_para[0 * 15 + 14] = 0;

	//gaussion_type = 1, variance = 0.0484
	gaussion_para[1 * 15 + 0] = 0;
	gaussion_para[1 * 15 + 1] = 0;
	gaussion_para[1 * 15 + 2] = 0;
	gaussion_para[1 * 15 + 3] = 0;
	gaussion_para[1 * 15 + 4] = 0;
	gaussion_para[1 * 15 + 5] = 0;
	gaussion_para[1 * 15 + 6] = 0;
	gaussion_para[1 * 15 + 7] = 1;
	gaussion_para[1 * 15 + 8] = 0;
	gaussion_para[1 * 15 + 9] = 0;
	gaussion_para[1 * 15 + 10] = 0;
	gaussion_para[1 * 15 + 11] = 0;
	gaussion_para[1 * 15 + 12] = 0;
	gaussion_para[1 * 15 + 13] = 0;
	gaussion_para[1 * 15 + 14] = 0;

	//gaussion_type = 2, variance = 0.187
	gaussion_para[2 * 15 + 0] = 0;
	gaussion_para[2 * 15 + 1] = 0;
	gaussion_para[2 * 15 + 2] = 0;
	gaussion_para[2 * 15 + 3] = 0;
	gaussion_para[2 * 15 + 4] = 0;
	gaussion_para[2 * 15 + 5] = 0;
	gaussion_para[2 * 15 + 6] = 0.00375;
	gaussion_para[2 * 15 + 7] = 0.9925;
	gaussion_para[2 * 15 + 8] = 0.00375;
	gaussion_para[2 * 15 + 9] = 0;
	gaussion_para[2 * 15 + 10] = 0;
	gaussion_para[2 * 15 + 11] = 0;
	gaussion_para[2 * 15 + 12] = 0;
	gaussion_para[2 * 15 + 13] = 0;
	gaussion_para[2 * 15 + 14] = 0;

	//gaussion_type = 3, variance = 0.567
	gaussion_para[3 * 15 + 0] = 0;
	gaussion_para[3 * 15 + 1] = 0;
	gaussion_para[3 * 15 + 2] = 0;
	gaussion_para[3 * 15 + 3] = 0;
	gaussion_para[3 * 15 + 4] = 0.000005;
	gaussion_para[3 * 15 + 5] = 0.004073;
	gaussion_para[3 * 15 + 6] = 0.184855;
	gaussion_para[3 * 15 + 7] = 0.622134;
	gaussion_para[3 * 15 + 8] = 0.184855;
	gaussion_para[3 * 15 + 9] = 0.004073;
	gaussion_para[3 * 15 + 10] = 0.000005;
	gaussion_para[3 * 15 + 11] = 0;
	gaussion_para[3 * 15 + 12] = 0;
	gaussion_para[3 * 15 + 13] = 0;
	gaussion_para[3 * 15 + 14] = 0;

	//gaussion_type = 4, variance = 1.99
	gaussion_para[4 * 15 + 0] = 0.000463;
	gaussion_para[4 * 15 + 1] = 0.002312;
	gaussion_para[4 * 15 + 2] = 0.009015;
	gaussion_para[4 * 15 + 3] = 0.027441;
	gaussion_para[4 * 15 + 4] = 0.065211;
	gaussion_para[4 * 15 + 5] = 0.121007;
	gaussion_para[4 * 15 + 6] = 0.175343;
	gaussion_para[4 * 15 + 7] = 0.198417;
	gaussion_para[4 * 15 + 8] = 0.175343;
	gaussion_para[4 * 15 + 9] = 0.121007;
	gaussion_para[4 * 15 + 10] = 0.065211;
	gaussion_para[4 * 15 + 11] = 0.027441;
	gaussion_para[4 * 15 + 12] = 0.00915;
	gaussion_para[4 * 15 + 13] = 0.002312;
	gaussion_para[4 * 15 + 14] = 0.000463;

	//gaussion_type = 5, variance = 7.41
	gaussion_para[5 * 15 + 0] = 0.050044;
	gaussion_para[5 * 15 + 1] = 0.056323;
	gaussion_para[5 * 15 + 2] = 0.062247;
	gaussion_para[5 * 15 + 3] = 0.067555;
	gaussion_para[5 * 15 + 4] = 0.071995;
	gaussion_para[5 * 15 + 5] = 0.075343;
	gaussion_para[5 * 15 + 6] = 0.077427;
	gaussion_para[5 * 15 + 7] = 0.078134;
	gaussion_para[5 * 15 + 8] = 0.077427;
	gaussion_para[5 * 15 + 9] = 0.075343;
	gaussion_para[5 * 15 + 10] = 0.071995;
	gaussion_para[5 * 15 + 11] = 0.067555;
	gaussion_para[5 * 15 + 12] = 0.062247;
	gaussion_para[5 * 15 + 13] = 0.056323;
	gaussion_para[5 * 15 + 14] = 0.050044;

	float bias = 1.0 / 800;
	vec2 tex_base = vec2(0, 0);
	if(blur_type == 1) tex_base = vec2(0, 1);
	if(blur_type == 2) tex_base = vec2(1, 0);

	vec2 tex[15];
	for(int i = 0; i < 15; i ++)
		tex[i] = texcoord + tex_base * bias * (i - 7);

	vec4 blur_color = vec4(0, 0, 0, 1);
	bool is_margin = false;
	vec4 max_neighbour_color = vec4(0, 0, 0, 1);
	for(int i = 0; i < 15; i ++){
		vec4 neighbour_color = texture2D(map_toblur, tex[i]);
		if(neighbour_color.x > max_neighbour_color.x) max_neighbour_color = neighbour_color;
		blur_color = blur_color + neighbour_color * gaussion_para[gaussion_type * 15 + i];
		if(neighbour_color == vec4(0, 0, 0, 1)) is_margin = true;
	}

	if(is_margin) blur_color = texture2D(map_toblur, tex[7]);
	if(mode == 6) blur_color = texture2D(map_toblur, tex[7]);
	blur_color.a = 1;
	fColor = blur_color;
}