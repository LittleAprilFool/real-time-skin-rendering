#version 400

in vec4 vPosition;
in vec2 vTexcoord;
in vec4 vNormal;
in vec4 vT;
out vec2 texcoord;

uniform mat4 depthMVP;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(){
	//gl_Position = projection * model_view * (vPosition + vec4(model_pos, 0.0));
	gl_Position = vPosition;
	texcoord = vTexcoord;
}