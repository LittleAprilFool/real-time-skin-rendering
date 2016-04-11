#version 400

in vec4 position;
in vec2 texcoord;
in vec4 normal;
in vec4 tangent;

layout(location = 0) out vec4 fColor;

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

uniform mat4 depth_model_matrix;
uniform mat4 depth_view_matrix;
uniform mat4 depth_projection_matrix;

uniform sampler2D map_kd;
uniform sampler2D map_bump;
uniform sampler2D map_rendered;

vec3 GetTBNTransformedLight()
{
	vec4 T = tangent;
	vec3 N = normalize(normal.xyz);
	vec3 B = normalize(cross(N, T.xyz)) * T.w;
	mat3 TBN = mat3(T.x, B.x, N.x, T.y, B.y, N.y, T.z, B.z, N.z);
	vec3 light = normalize(position.xyz - Light.position.xyz);
	light = normalize(TBN * light);
	return light;
}

float GetVisibility()
{
	mat4 biasMatrix = mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	mat4 depthMVP = depth_projection_matrix * depth_view_matrix * depth_model_matrix;
	
	mat4 depthBiasMVP = biasMatrix*depthMVP;

	vec4 shadowcoord = depthBiasMVP * position;
	float visibility = 0.4;
	if( texture2D(map_rendered, shadowcoord.xy).z < shadowcoord.z) visibility = 0.2;
	return visibility;
}

vec3 ComputeDiffuseColor(vec3 kd, vec3 light, vec3 norm)
{
	return Material.kd * Light.ld * max(dot(light,norm), 0);
}

vec3 ComputeWrapDiffuseColor(vec3 kd, vec3 light, vec3 norm)
{
	float wrap = 0.2;
	float NdotL = dot(norm, light);
	float NdotL_wrap = (NdotL + wrap) / (1 + wrap);
	return Material.kd * Light.ld * max(NdotL_wrap, 0);
}

vec3 ComputeScatterColor(vec3 kd, vec3 light, vec3 norm)
{
	float wrap = 0.2;
	float NdotL = dot(norm, light);
	float NdotL_wrap = (NdotL + wrap) / (1 + wrap);
	float scatter_width = 0.3;
	vec3 scatter_color = vec3(0.15,0,0);
	float scatter_factor = smoothstep(0, scatter_width, NdotL_wrap) * 
					smoothstep(2 * scatter_width, scatter_width, NdotL_wrap);
	return scatter_factor * scatter_color;
}

vec3 ComputeSpecularColor(vec3 kd, vec3 light, vec3 norm)
{
	vec3 re = reflect(-light, norm);
	float factor = pow(max(dot(re, position.xyz),0), Material.shininess);
	return Light.ls * Material.ks * factor;
}

void main()
{
	vec3 kd = texture2D(map_kd, texcoord).xyz;

	vec3 light = GetTBNTransformedLight();

	float visibility = GetVisibility();

	vec3 norm = 2 * texture2D(map_bump, texcoord).xyz - vec3(1.0);

	vec3 diffuse = ComputeDiffuseColor(kd, light, norm);

//	vec3 diffuse_wrap = ComputeWrapDiffuseColor(kd, light, norm);

	vec3 ambient = Material.ka * Light.la;

//	vec3 scatter = ComputeScatterColor(kd, light, norm);

	vec3 spec = ComputeSpecularColor(kd, light, norm);

	visibility = 0.4;
	vec3 light_intensity;
	if (mode == 1) light_intensity = kd * 0.5 + diffuse * visibility + ambient * visibility;
//	if (mode == 2) light_intensity = wrapLight(light, norm.xyz) * visibility + kdColor.xyz * 0.6;
//	if (mode == 3) light_intensity = wrapLight(light, norm.xyz);
	fColor = vec4(light_intensity, 1);
}