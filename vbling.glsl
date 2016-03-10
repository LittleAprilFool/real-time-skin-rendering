#version 400

in  vec4 vPosition;
//in vec4 vTexcoord;
out float gn;
//out vec4 texcoord;
uniform mat4 model_view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * model_view * vPosition;
  //texcoord = vTexcoord;
  gn = 0;
}