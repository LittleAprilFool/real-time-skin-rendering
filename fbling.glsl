#version 400

in float gn;
in vec2 texcoord;
in vec3 lcolor;
out vec4 fColor;
uniform float translucency;
uniform sampler2D map_kd;
uniform sampler2D map_bump;

void main()
{
  fColor = texture2D(map_kd, texcoord);
  vec4 bColor = texture2D(map_bump, texcoord);
  fColor.xyz = fColor.xyz * 0.5 + lcolor.xyz * 0.4 + bColor.xyz * 0.1;
  fColor.a = 1.0;
}