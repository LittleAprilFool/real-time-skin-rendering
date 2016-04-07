#pragma once
#include "Scene.h"
class HeadScene :
	public Scene
{
public:
	HeadScene();
	~HeadScene();
	void RenderScene();
	void InitScene();
	void KeyboardFunction(int key, int action);


	float scale_factor;
	vec3 rotate_factor;

	float translucency_value;
	int shading_mode;
	int display_mode;
	vec3 light_position;
	vec3 light_color;
	vec3 global_ambient;
	vec3 Kd;

private:
	GLuint loc_model_view;
	GLuint loc_projection;
	GLuint loc_translucency;
	GLuint loc_light_pos;
	GLuint loc_light_color;
	GLuint loc_Kd;
	GLuint loc_global_ambient;
	GLuint loc_mode;
	GLuint loc_map_kd;
	GLuint loc_map_bump;
	GLuint texture_kd_ID;
	GLuint texture_bump_ID;
	void InitParameters_();
	void GetUniformLocations_();
};

