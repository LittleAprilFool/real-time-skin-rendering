#pragma once
#include "obj3d.h"
class Head :
	public Object
{
public:
	Head();
	~Head();
	void InitBuffer();
	void RenderObject();

	float scale_factor;
	vec3 rotate_factor;

	float translucency_value;
	int shading_mode;
	int display_mode;
	vec3 light_position;
	vec3 light_color;
	vec3 global_ambient;
	vec3 Kd;
	GLuint loc_map_kd;
	GLuint loc_map_bump;
private:
	GLuint loc_model_view;
	GLuint loc_projection;
	GLuint loc_translucency;
	GLuint loc_light_pos;
	GLuint loc_light_color;
	GLuint loc_Kd;
	GLuint loc_global_ambient;
	GLuint loc_mode;
	void InitParameters_();
};

class Cube :
	public Object
{
public:
	Cube();
	~Cube();
	void InitBuffer();
	void RenderObject();
};
