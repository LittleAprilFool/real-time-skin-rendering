#pragma once
#include "Scene.h"
class HeadScene :
	public Scene
{
public:
	HeadScene();
	~HeadScene();
	void RenderScene();
	void RenderLight();
	void InitScene(int width, int height);
	void KeyboardFunction(int key, int action);
	void MouseControl(int button, int action, int mods);
	void CursorPosition(double xpos, double ypos);


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
	void InitParameters_();
	void InitGLFunc_();

	void RenderObject_(Object* obj, GLuint shader_ID);
	void DrawArray_(int face_number);
	void GetUniformLocations_(GLuint shader_ID);
	void TransferDataToShader_();

	void UpdateModelMatrix_();

	GLuint loc_model;
	GLuint loc_view;
	GLuint loc_projection;

	GLuint loc_light_pos;
	GLuint loc_light_la;
	GLuint loc_light_ld;
	GLuint loc_light_ls;

	GLuint loc_material_ka;
	GLuint loc_material_kd;
	GLuint loc_material_ks;
	GLuint loc_material_shininess;

	GLuint loc_mode;
	GLuint loc_eye_pos;
	
	GLuint loc_map_kd;
	GLuint loc_map_bump;
	GLuint loc_map_rendered;
	GLuint loc_map_scattered;
	GLuint loc_map_light;
	GLuint loc_map_blur;

	GLuint loc_depth_model_matrix;
	GLuint loc_depth_view_matrix;
	GLuint loc_depth_projection_matrix;

	
	GLuint texture_kd_ID;
	GLuint texture_bump_ID;
	GLuint texture_rendered_ID;
	GLuint texture_depth_ID;
	GLuint texture_scattered_ID;
	GLuint texture_light_ID;
	GLuint texture_blur_ID;
	
	GLuint buffer_depth_ID;
	GLuint fbo_shadow_ID;
	GLuint fbo_texture_ID;
	GLuint fbo_blur_ID;
	
	mat4 projection_matrix;
	mat4 model_matrix;
	mat4 view_matrix;

	mat4 depth_projection_matrix;
	mat4 depth_view_matrix;
	mat4 depth_model_matrix;

	vec3 light_la;
	vec3 light_ld;
	vec3 light_ls;

	vec3 material_ka;
	vec3 material_kd;
	vec3 material_ks;
	float material_shininess;
	
	vec3 eye;
	vec3 at;
	vec3 up;
	
	Object* head;
	Object* cube;
	vec2 mouse_position;
	vec2 mouse_position_o;
	bool mouse_press_left;
	bool mouse_press_right;
	int scene_width;
	int scene_height;

	ShaderProgram* shader_bling;
	ShaderProgram* shader_texture;
	ShaderProgram* shader_test;
	ShaderProgram* shader_shadow;
	ShaderProgram* shader_blur;

	GLuint shader_bling_ID;
	GLuint shader_texture_ID;
	GLuint shader_test_ID;
	GLuint shader_shadow_ID;
	GLuint shader_blur_ID;
};

