#version 400

in vec2 texcoord;
layout(location = 0) out vec4 fColor;

uniform int gaussion_type;
uniform sampler2D map_toadd;
uniform sampler2D map_add;

void main()
{
	vec3 blur_weight[6];
	blur_weight[0] = vec3(0.233, 0.455, 0.649);
	blur_weight[1] = vec3(0.100, 0.336, 0.344);
	blur_weight[2] = vec3(0.118, 0.198, 0);
	blur_weight[3] = vec3(0.113, 0.007, 0.007);
	blur_weight[4] = vec3(0.358, 0.004, 0);
	blur_weight[5] = vec3(0.078, 0, 0);

	fColor = vec4(texture2D(map_toadd, texcoord).xyz * blur_weight[gaussion_type], 1) + texture2D(map_add, texcoord);
}