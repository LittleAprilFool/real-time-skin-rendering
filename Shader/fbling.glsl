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

uniform vec3 eye_position;

uniform mat4 depth_model_matrix;
uniform mat4 depth_view_matrix;
uniform mat4 depth_projection_matrix;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

uniform sampler2D map_kd;
uniform sampler2D map_bump;
uniform sampler2D map_depth;
uniform sampler2D map_scattered;
uniform sampler2D map_blur;
uniform sampler2D map_afteradd;

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
	if(visibility < 0) visibility = 0;
	if(visibility > 1) visibility = 1;
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
	    current_depth = texture(map_depth, pos).z;
		//current_depth = texture(map_depth, texcoord).z;
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

vec3 ComputeScatterColor(float thickness)
{
	float value = thickness * 20;
	float scattered_x = thickness * 4;
	if(scattered_x >= 1) scattered_x = 1;
	if(scattered_x <= 0.01) return vec3(0,0,0);
	//return texture(map_scattered, vec2(scattered_x, 0.5)).xyz;
	if(value < 0.5) value = 1 - value;
	if(value > 1) value = 1;
	value = 1 - value;
	return vec3(value, 0,0);
	//return texture(map_scattered, vec2(value, 0.5)).xyz;
}

void main()
{
	vec3 kd = texture(map_kd, texcoord).xyz;

	vec3 light = GetTBNTransformedVector(Light.position);

	vec3 eye = GetTBNTransformedVector(eye_position);

	vec3 norm = 2 * texture(map_bump, texcoord).xyz - vec3(1.0);

	float depth = GetDepth(light, norm);

	float thickness = GetThickness(depth);

	float visibility = GetVisibility(light, norm, depth);

	vec3 scatter = ComputeScatterColor(thickness);

	vec3 light_intensity;
	light_intensity = texture(map_afteradd, texcoord).xyz;
	if (mode == 1) light_intensity = kd * light_intensity * visibility + scatter * 0.1;
	if (mode == 2) light_intensity = kd * light_intensity + scatter * 0.1;
	if (mode == 3) light_intensity = scatter;
	if (mode == 5) light_intensity = kd * light_intensity + scatter;
	if (mode == 6 || mode == 4) light_intensity = kd * light_intensity * visibility + scatter * 0.1;
	fColor = vec4(light_intensity, 1);
}