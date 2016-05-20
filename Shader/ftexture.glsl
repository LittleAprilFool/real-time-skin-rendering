#version 400
in vec4 position;
in vec4 normal;
in vec4 tangent;
in vec2 texcoord;

out vec4 fColor;

struct LightInfo {
	vec3 position;
	vec3 la;
	vec3 ld;
	vec3 ls;
};

struct MaterialInfo {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float shininess;
};

uniform LightInfo Light;
uniform MaterialInfo Material;

uniform float mode;

uniform vec3 eye_position;

uniform mat4 depth_model_matrix;
uniform mat4 depth_view_matrix;
uniform mat4 depth_projection_matrix;

uniform sampler2D map_bump;
uniform sampler2D map_kd;
uniform sampler2D map_beckmann;

vec3 GetTBNTransformedVector(vec3 vector)
{
	vec4 T = tangent;
	vec3 N = normalize(normal.xyz);
	vec3 B = normalize(cross(N, T.xyz)) * T.w;
	mat3 TBN = mat3(T.x, B.x, N.x, T.y, B.y, N.y, T.z, B.z, N.z);
	vec3 transform_vector = normalize(vector.xyz - position.xyz);
	transform_vector = normalize(TBN * transform_vector);
	return transform_vector;
}

vec3 ComputeDiffuseColor(vec3 light, vec3 norm)
{
	return Material.kd * Light.ld * max(dot(light,norm), 0);
}

vec3 ComputeSpecularColor(vec3 light, vec3 eye, vec3 norm)
{
	vec3 re = reflect(-light, norm);
	float factor = pow(max(dot(re, eye),0), Material.shininess);
	return Light.ls * Material.ks * factor;
}

float FresnelReflectance(vec3 H, vec3 V, float F0)
{
	float base = 1.0 - dot(H, V);
	float exponential = pow(base, 5.0);
	return exponential + F0 * (1 - exponential);
}

vec3 SkinSpecular(vec3 N, vec3 L, vec3 V, float m, float rho_s)
{
	float result = 0;
	float ndotl = dot(N, L);
	if(ndotl > 0)
	{
		vec3 h = L + V;
		vec3 H = normalize(h);
		float ndoth = dot(N, H);
		float PH = pow(2 * texture(map_beckmann, vec2(ndoth,m)).x,10);
		float F = FresnelReflectance(H, V, 0.028);
		float frSpec = max(PH * F / dot(h, h), 0);
		result = ndotl * rho_s * frSpec;
	}
	return vec3(result, result, result);
}

void main()
{
   vec3 light = GetTBNTransformedVector(Light.position);
   vec3 norm = 2 * texture(map_bump, texcoord).xyz - vec3(1.0);
   vec3 eye = GetTBNTransformedVector(eye_position);

   vec3 diffuse = ComputeDiffuseColor(light, norm);
   vec3 ambient = Material.ka * Light.la;
   vec3 specular = ComputeSpecularColor(light, eye, norm);

   vec3 skin_specular = SkinSpecular(norm, light, eye, 10, 20);

   fColor = vec4(diffuse + ambient + specular , 1);
  // if (mode == 4) fColor = vec4(diffuse + ambient + skin_specular , 1);
}