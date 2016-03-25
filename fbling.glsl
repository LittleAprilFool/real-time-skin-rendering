#version 400

in vec2 texcoord;
in vec4 position;
in vec4 normal;
in vec4 T;
in vec3 N;
out vec4 fColor;

uniform vec3 model_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 Kd;
uniform vec3 global_ambient;

uniform sampler2D map_kd;
uniform sampler2D map_bump;

void main()
{
  vec4 kdColor = texture2D(map_kd, texcoord);
  vec4 bnormal = 2 * texture2D(map_bump, texcoord) - 1.0;
  
  vec3 nnn = normalize(normal.xyz);
  
  vec4 lColor;

  //compute the binormal
  vec3 B = normalize(cross(nnn, T.xyz)) * T.w;

  mat3 TBN = mat3(
	T.x, B.x, nnn.x,
	T.y, B.y, nnn.y,
	T.z, B.z, nnn.z);

  //compute L
  vec3 L = normalize(position.xyz - light_pos);
 
  L = normalize(TBN * L.xyz);

  //compute diffuseColor
  vec3 diffuseColor = Kd * light_color * max(dot(L, bnormal.xyz), 0);
  
  //compute ambientColor 
  vec3 ambientColor = Kd * global_ambient;

  lColor.xyz = diffuseColor + ambientColor;
  lColor.a = 1.0;

  fColor = lColor;
  //fColor.xyz = lColor.xyz * 0.3 + kdColor.xyz * 0.7;
  //fColor.a = 1.0;
}