#version 400

in vec2 vTexcoord;

out vec2 texcoord;

void main(){ 
	texcoord = vTexcoord;
	gl_Position = vec4(vTexcoord.x * 2 - 1, vTexcoord.y * 2 - 1, 1, 1);
}