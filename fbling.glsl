#version 400

in vec2 texcoord;
in vec4 position;
in vec4 normal;
in vec4 T;
out vec4 fColor;

uniform vec3 model_pos;

struct LightInfo{
	vec3 position;
	vec3 color;
};

uniform LightInfo Light;

uniform vec3 Kd;
uniform vec3 global_ambient;

uniform sampler2D map_kd;
uniform sampler2D map_bump;

vec3 phoneModel(vec3 light, vec3 norm)
{
	vec3 diffuse = Kd * Light.color * max(dot(light, norm), 0);
	vec3 ambient = Kd * global_ambient;
	return diffuse + ambient;
}

void main()
{
  vec4 kdColor = texture2D(map_kd, texcoord);
  vec4 norm = 2 * texture2D(map_bump, texcoord) - 1.0;
  
  vec3 N = normalize(normal.xyz);
  
  vec4 lColor;

  //compute the binormal
  vec3 B = normalize(cross(N, T.xyz)) * T.w;

  mat3 TBN = mat3(
	T.x, B.x, N.x,
	T.y, B.y, N.y,
	T.z, B.z, N.z);

  //compute light
  vec3 light = normalize(position.xyz - Light.position);
 
  light = normalize(TBN * light.xyz);

  fColor.xyz = phoneModel(light, norm.xyz) * 0.3 + kdColor.xyz * 0.7;
  fColor.a = 1.0;
}