#version 400

//in vec2 texcoord;
//in vec4 position;
//in vec4 normal;
//in vec4 T;
out vec4 fColor;

uniform vec3 model_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 Kd;
uniform vec3 global_ambient;

uniform sampler2D map_kd;
uniform sampler2D map_bump;

//vec3 phoneModel(vec3 light, vec3 norm) {
//	vec3 diffuse = Kd * light_color * max(dot(light, norm), 0);
//	vec3 ambient = Kd * global_ambient;
//	vec3 spec = vec3(0,0,0);
//	return diffuse + ambient + spec;
//}

void main()
{
//  vec4 kdColor = texture2D(map_kd, texcoord);
//  vec4 bnormal = 2 * texture2D(map_bump, texcoord) - 1.0;
  
//  vec3 N = normalize(normal.xyz);
  
//  vec4 lColor;

  //compute the binormal
//  vec3 B = normalize(cross(N, T.xyz)) * T.w;

// mat3 TBN = mat3(
//	T.x, B.x, N.x,
//	T.y, B.y, N.y,
//	T.z, B.z, N.z);

  //compute L
  //vec3 L = normalize(position.xyz - light_pos);
 
  //L = normalize(TBN * L.xyz);

  //vec3 phoneShader = phoneModel(L, bnormal.xyz);

  //fColor.xyz = phoneShader * 0.4 + kdColor.xyz * 0.6;
  
  //fColor.a = 1.0;
  fColor = vec4(1,1,1,1);

}