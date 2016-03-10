#version 400

in float gn;
out vec4 fColor;
uniform float translucency;
//uniform sampler2D map_kd;

void main()
{
  fColor = vec4(0.0f, 1.0f, 0.5f, translucency);
}