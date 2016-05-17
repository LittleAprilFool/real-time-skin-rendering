#version 400
in vec2 texcoord;
in vec4 shadowcoord;
out vec4 fColor;

uniform sampler2D map_depth;

void main()
{
	float xpos = shadowcoord.x;
	float ypos = shadowcoord.y;

	vec2 pos = vec2(xpos, ypos);

	float current_depth = texture(map_depth, pos).x;

	//if(current_depth < depth) depth = current_depth;
	//float thickness = shadowcoord.z - depth;

//	float value = thickness * 20;
	fColor = vec4(current_depth, 0, 0, 1);
//	if(current_depth != 0.5) fColor = vec4(0, 1, 0, 1);
}