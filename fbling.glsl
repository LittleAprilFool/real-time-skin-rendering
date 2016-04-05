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

uniform float mode;

vec3 phoneModel(vec3 light, vec3 norm)
{
	vec3 diffuse = Kd * Light.color * max(dot(light, norm), 0);
	vec3 ambient = Kd * global_ambient;
	return diffuse + ambient;
}

vec3 wrapLight(vec3 L, vec3 N)
{
	float wrap = 0.2;
	float NdotL = dot(N, L);
	float NdotL_wrap = (NdotL + wrap)/(1 + wrap);
	
	vec3 diffuse = Kd * Light.color * max(NdotL_wrap, 0);

	float scatterWidth = 0.3;
	vec3 scatterColor = vec3(0.15, 0, 0);

	float scatter = smoothstep(0, scatterWidth, NdotL_wrap) * 
					smoothstep(2 * scatterWidth, scatterWidth, NdotL_wrap);
    
	vec3 ambient = Kd * global_ambient;

	return ambient + diffuse + scatter * scatterColor;
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

  if (mode == 1) fColor.xyz = phoneModel(light, norm.xyz) * 0.3 + kdColor.xyz * 0.7;
  if (mode == 2) fColor.xyz = wrapLight(light, norm.xyz) * 0.3 + kdColor.xyz * 0.7;
  if (mode == 3) fColor.xyz = wrapLight(light, norm.xyz);
  fColor.a = 1.0;
}