#version 400

in  vec4 vPosition;
in vec2 vTexcoord;
in vec4 vNormal;
out vec2 texcoord;
out vec4 position;
out vec4 normal;

uniform vec3 model_pos;
uniform mat4 model_view;
uniform mat4 projection;

void main()
{
  //compute position
  gl_Position = projection * model_view * (vPosition + vec4(model_pos, 0.0));

  texcoord = vTexcoord;
  position = vPosition;
  normal = vNormal;
}