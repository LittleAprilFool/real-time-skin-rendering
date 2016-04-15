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
uniform sampler2D map_scattered;

vec3 GetTBNTransformedLight()
{
	vec4 T = tangent;
	vec3 N = normalize(normal.xyz);
	vec3 B = normalize(cross(N, T.xyz)) * T.w;
	mat3 TBN = mat3(T.x, B.x, N.x, T.y, B.y, N.y, T.z, B.z, N.z);
	vec3 light = normalize(Light.position.xyz - position.xyz);
	light = normalize(TBN * light);
	return light;
}

float GetVisibility(vec3 light, vec3 norm, vec4 shadowcoord)
{
	float visibility = 1;
	float bias = 0.005 * tan(acos(dot(light, norm)));
	bias = clamp(bias, 0.0f, 0.01f);
	float factor = (shadowcoord.z-bias) / texture(map_rendered, shadowcoord.xy).z;

	if( factor > 1) visibility = 1 / factor;
	return visibility;
}

vec4 GetShadowCoord()
{
	mat4 biasMatrix = mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	mat4 depthMVP = depth_projection_matrix * depth_view_matrix * depth_model_matrix;
	
	mat4 depthBiasMVP = biasMatrix*depthMVP;
	vec4 shadowcoord = depthMVP * position;
	shadowcoord = shadowcoord / 3 + 0.5;
	return shadowcoord;
}

float GetDepth(vec3 light, vec3 norm, vec4 shadowcoord)
{
	float visibility = 1;
	float bias = 0.005 * tan(acos(dot(light, norm)));
	bias = clamp(bias, 0.0f, 0.01f);
	float depth = abs((shadowcoord.z-bias) - texture(map_rendered, shadowcoord.xy).z);
	return depth;
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

vec3 ScatteredTestColor(float depth)
{
	float depth_u = depth * 5;
	if(depth_u < 0.1) depth_u = 0.1;
	if(depth_u > 0.3) depth_u = 0.3;
//	if(depth_u > 0.5) return vec3(1,0,0);
//	if(depth_u > 0.3) return vec3(0,1,0);
//	if(depth_u > 0.1) return vec3(0,0,1);
	return vec3(depth_u,0,0);
	return texture(map_scattered, vec2(depth_u, 0.5)).xyz;
}

void main()
{
	vec3 kd = texture(map_kd, texcoord).xyz;

	vec3 light = GetTBNTransformedLight();

	vec3 norm = 2 * texture(map_bump, texcoord).xyz - vec3(1.0);

	vec4 shadowcoord = GetShadowCoord();

	float depth = GetDepth(light, norm, shadowcoord);

	float visibility = GetVisibility(light, norm, shadowcoord);

	vec3 diffuse = ComputeDiffuseColor(kd, light, norm);

//	vec3 diffuse_wrap = ComputeWrapDiffuseColor(kd, light, norm);

	vec3 ambient = Material.ka * Light.la;

//	vec3 scatter = ComputeScatterColor(kd, light, norm);

	vec3 spec = ComputeSpecularColor(kd, light, norm);

	vec3 light_intensity;
//	light_intensity = diffuse * visibility + ambient * visibility;
	light_intensity = ScatteredTestColor(depth);
//	visibility = 0.2;
//	if (mode == 1) light_intensity = kd * 0.5 + diffuse * visibility + ambient * visibility;
//	if (mode == 2) light_intensity = wrapLight(light, norm.xyz) * visibility + kdColor.xyz * 0.6;
//	if (mode == 3) light_intensity = wrapLight(light, norm.xyz);
	fColor = vec4(light_intensity, 1);
}