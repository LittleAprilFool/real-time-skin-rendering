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

uniform vec3 eye_position;

uniform mat4 depth_model_matrix;
uniform mat4 depth_view_matrix;
uniform mat4 depth_projection_matrix;

uniform sampler2D map_bump;
uniform sampler2D map_kd;

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

void main()
{
   vec3 light = GetTBNTransformedVector(Light.position);
   vec3 norm = 2 * texture(map_bump, texcoord).xyz - vec3(1.0);
   vec3 eye = GetTBNTransformedVector(eye_position);

   vec3 diffuse = ComputeDiffuseColor(light, norm);
   vec3 ambient = Material.ka * Light.la;
   vec3 specular = ComputeSpecularColor(light, eye, norm);

   fColor = vec4(diffuse + ambient + specular , 1);
}