#version 400
precision highp float;
precision highp int;

in vec4 position;
in vec2 texcoord;
in vec4 normal;
in vec4 tangent;
in vec4 shadowcoord;

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

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

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

float GetVisibility(vec3 light, vec3 norm, float depth)
{
	float visibility = 1;
	float bias = 0.005 * tan(acos(dot(light, norm)));
	bias = clamp(bias, 0.0f, 0.01f);
	float factor = 1;
	if(depth != 0) factor = (shadowcoord.z-bias) / depth;
	if( factor > 1) visibility = 1 / factor;
	visibility = visibility - 0.5;
	visibility = visibility * 2;
	visibility = 1;
	if(shadowcoord.z - bias * 2 > depth) visibility = 0.2;
	if(visibility < 0) visibility = 0;
	return visibility;
}

float GetDepth(vec3 light, vec3 norm)
{
	float xpos = shadowcoord.x;
	float ypos = shadowcoord.y;
	int effective_depth_number = 0;
	float depth_sum = 0;
	float bias = 0.005;
	float depth = 1;
	float current_depth = 1;

	for(int i = -1; i < 2; i ++)
	for(int j = -1; j < 2; j ++)
	{
		vec2 pos = vec2(xpos + i * bias, ypos + j * bias);
	    current_depth = texture(map_rendered, pos).z;
		if(current_depth < depth) depth = current_depth;
	}

	return depth;
}

float GetThickness(float depth)
{
	float thickness = shadowcoord.z - depth;
	if(thickness < 0.005) thickness = 0;
	return thickness;
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

//vec3 ComputeScatterColor(vec3 kd, vec3 light, vec3 norm)
//{
//	float wrap = 0.2;
//	float NdotL = dot(norm, light);
//	float NdotL_wrap = (NdotL + wrap) / (1 + wrap);
//	float scatter_width = 0.3;
//	vec3 scatter_color = vec3(0.15,0,0);
//	float scatter_factor = smoothstep(0, scatter_width, NdotL_wrap) * 
//					smoothstep(2 * scatter_width, scatter_width, NdotL_wrap);
//	return scatter_factor * scatter_color;
//}

vec3 ComputeSpecularColor(vec3 kd, vec3 light, vec3 norm)
{
	vec3 re = reflect(-light, norm);
	float factor = pow(max(dot(re, position.xyz),0), Material.shininess);
	return Light.ls * Material.ks * factor;
}

vec3 ComputeScatterColor(float thickness)
{
	float scattered_x = thickness * 4;
	if(scattered_x >= 1) scattered_x = 1;
	if(scattered_x == 0) scattered_x = 1;
	return texture(map_scattered, vec2(scattered_x, 0.5)).xyz;
}

void main()
{
	vec3 kd = texture(map_kd, texcoord).xyz;

	vec3 light = GetTBNTransformedLight();

	vec3 norm = 2 * texture(map_bump, texcoord).xyz - vec3(1.0);

	float depth = GetDepth(light, norm);

	float thickness = GetThickness(depth);

	float visibility = GetVisibility(light, norm, depth);

	vec3 diffuse = ComputeDiffuseColor(kd, light, norm);

//	vec3 diffuse_wrap = ComputeWrapDiffuseColor(kd, light, norm);

	vec3 ambient = Material.ka * Light.la;

	vec3 scatter = ComputeScatterColor(thickness);

	vec3 spec = ComputeSpecularColor(kd, light, norm);

	vec3 light_intensity;
//	light_intensity = diffuse * visibility + ambient * visibility;
//	light_intensity = ComputeScatterColor(depth);
	if (mode == 1) light_intensity = kd * 0.8 + diffuse * visibility + ambient * visibility + 0.3 * scatter;
	if (mode == 2) light_intensity = kd * 0.8 + diffuse * visibility + ambient * visibility;
	if (mode == 3) light_intensity = scatter;
	if (mode == 4) light_intensity = diffuse * visibility + ambient * visibility;
	if (mode == 5) light_intensity = diffuse + ambient;
//	if (mode == 3) light_intensity = wrapLight(light, norm.xyz);
	fColor = vec4(light_intensity, 1);
}