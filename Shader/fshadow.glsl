#version 400

in vec2 texcoord;
in vec4 shadowcoord;

out vec4 fColor;

uniform sampler2D map_depth;

void main()
{
	float xpos = shadowcoord.x;
	float ypos = shadowcoord.y;

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

	float visibility = 1;
	float factor = 1;
	if(depth != 0) factor = (shadowcoord.z - bias) / depth;
	if(factor > 1) visibility = 1 / factor;
	visibility = (visibility - 0.5) * 2;
	if(visibility < 0) visibility = 0;
	if(visibility > 1) visibility = 1;

	fColor = vec4(vec3(visibility), 1);
}