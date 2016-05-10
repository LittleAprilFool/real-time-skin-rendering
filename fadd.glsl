#version 400
in vec2 texcoord;
layout(location = 0) out vec4 fColor;

uniform int gaussion_type;
uniform sampler2D map_toadd;
uniform sampler2D map_add;

void main()
{
	float blur_weight[4];
	blur_weight[0] = 0.07;
	blur_weight[1] = 0.18;
	blur_weight[2] = 0.21;
	blur_weight[3] = 0.29;
	
	//fColor = texture2D(map_toadd, texcoord) * blur_weight[gaussion_type] 
	//	+ texture2D(map_add, texcoord);	
	fColor = vec4(1,0,0,1);
}