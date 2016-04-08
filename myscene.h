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
	GLuint loc_model;
	GLuint loc_view;
	GLuint loc_projection;
	GLuint loc_translucency;
	GLuint loc_light_pos;
	GLuint loc_light_color;
	GLuint loc_Kd;
	GLuint loc_global_ambient;
	GLuint loc_mode;
	GLuint loc_map_kd;
	GLuint loc_map_bump;
	GLuint loc_map_rendered;
	GLuint loc_depthMVP;
	GLuint texture_kd_ID;
	GLuint texture_bump_ID;
	GLuint texture_rendered_ID;
	GLuint texture_depth_ID;
	GLuint depth_buffer_ID;
	void InitParameters_();
	void InitGLFunc_();
	void DrawArray_(int face_number);
	void RenderScene_(int face_number);
	void GetUniformLocations_(GLuint shader_ID);
	void InitFBO_();
	void TransferDataToShader_();
	GLuint FBO_ID;
	GLuint CreateRenderTexture_();
	GLuint CreateRenderTextureForShadow_();
	void PrintLoc_();
	void PassDepthMVP();
	mat4 projection_matrix;
	mat4 model_matrix;
	mat4 view_matrix;
	void UpdateModelMatrix_();
};

