#version 400

in vec4 vPosition;
in vec2 vTexcoord;
in vec4 vNormal;
in vec4 vT;
out vec2 texcoord;

void main(){
	gl_Position = vPosition * 5;
	gl_Position.a = 1;
	texcoord = vTexcoord;
}