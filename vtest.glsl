#version 400

in vec4 vPosition;
in vec2 vTexcoord;
in vec4 vNormal;
in vec4 vT;
out vec2 texcoord;

void main(){
	texcoord = vTexcoord;
	texcoord = texcoord * 2 - 1;
	gl_Position = vec4(texcoord, 1, 1);
	gl_Position.a = 1;
	texcoord = vTexcoord;
}