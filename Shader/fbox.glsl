#version 400

in vec2 texcoord;
out vec4 fColor;

uniform sampler2D map_sky;

void main(){
	fColor = texture(map_sky, texcoord);
}