#version 400
in vec2 texcoord;
in vec4 shadowcoord;
out vec4 fColor;

uniform sampler2D map_depth;
uniform sampler2D map_scattered;

void main()
{
	float xpos = shadowcoord.x;
	float ypos = shadowcoord.y;
	float depth_sum = 0;
	float bias = 0.005;
	float depth = 1;
	float current_depth = 1;
	for(int i = -1; i < 2; i ++)
	for(int j = -1; j < 2; j ++)
	{
		vec2 pos = vec2(xpos + i * bias, ypos + j * bias);
	    current_depth = texture(map_depth, pos).z;
		if(current_depth < depth) depth = current_depth;
	}

	float thickness = shadowcoord.z - depth;
	float value = thickness * 20;

	float scattered_x = thickness * 4;
	if(scattered_x <= 0.01) value = 0;
	if(value < 0.5) value = 1 - value;
	if(value > 1) value = 1;
	value = 1 - value;
	//fColor = texture(map_scattered, vec2(thickness * 20, 0));
	fColor = vec4(value, 0, 0, 1);
}