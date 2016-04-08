#version 400

in vec4 vPosition;
in vec2 vTexcoord;
in vec4 vNormal;
in vec4 vT;
out vec2 texcoord;

uniform vec3 model_pos;
uniform mat4 model_view;
uniform mat4 projection;

void main(){
	gl_Position = projection * model_view * (vPosition + vec4(model_pos, 0.0));
	texcoord = vTexcoord;
}