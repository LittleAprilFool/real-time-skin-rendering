#version 400

in  vec4 vPosition;
in vec2 vTexcoord;
in vec4 vNormal;
in vec4 vT;

out vec2 texcoord;
out vec4 position;
out vec4 normal;
out vec4 T;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
  //compute position
  gl_Position = projection_matrix * view_matrix * model_matrix * vPosition;

  texcoord = vTexcoord;
  position = vPosition;
  normal = vNormal;
  T = vT;
}