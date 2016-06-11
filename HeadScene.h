#pragma once
#include "Scene.h"
#include "Beckmann.h"
//#include "SkyBox.h"

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

	int shading_mode;
	int display_mode;
	vec3 light_position;
	vec3 light_color;
	vec3 global_ambient;
	vec3 Kd;

protected:
	void RenderObject_(Object* obj, GLuint shader_ID);
	void DrawArray_(int face_number);
	void GetUniformLocations_(GLuint shader_ID);
	void TransferDataToShader_();
	void RenderBlur_(int para, int rendered);
	void GaussionSum_(int method);

	void UpdateModelMatrix_();
	void CopyTexture_(GLuint tex_src, GLuint tex_dst);

private:
	void InitParameters_();
	void InitGLFunc_();

	Beckmann beckmann;

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
	GLuint loc_blur_type;
	GLuint loc_gaussion_type;
	GLuint loc_toadd_weight;
	
	GLuint loc_map_kd;
	GLuint loc_map_bump;
	GLuint loc_map_depth;
	GLuint loc_map_thickness;
	GLuint loc_map_scattered;
	GLuint loc_map_light;
	GLuint loc_map_blur;
	GLuint loc_map_toblur;
	GLuint loc_map_beckmann;
	GLuint loc_map_add;
	GLuint loc_map_toadd;
	GLuint loc_map_afteradd;
	GLuint loc_map_shadow;

	GLuint loc_depth_model_matrix;
	GLuint loc_depth_view_matrix;
	GLuint loc_depth_projection_matrix;

	
	GLuint texture_kd_ID;
	GLuint texture_bump_ID;
	GLuint texture_depth_ID;
	GLuint texture_thickness_ID;
	GLuint texture_scattered_ID;
	GLuint texture_light_ID;
	GLuint texture_blur_ID;
	GLuint texture_toblur_ID;
	GLuint texture_beckmann_ID;
	GLuint texture_add_ID;
	GLuint texture_toadd_ID;
	GLuint texture_afteradd_ID;
	GLuint texture_zero_ID;
	GLuint texture_shadow_ID;
	
	GLuint buffer_depth_ID;
	GLuint fbo_depth_ID;
	GLuint fbo_light_ID;
	GLuint fbo_blur_ID;
	GLuint fbo_beckmann_ID;
	GLuint fbo_add_ID;
	GLuint fbo_thickness_ID;
	GLuint fbo_shadow_ID;

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
	float test_mode;
	
	vec3 eye;
	vec3 at;
	vec3 up;
	
	Object* head;
	Object* cube;
//	SkyBox* skybox;
	vec2 mouse_position;
	vec2 mouse_position_o;
	bool mouse_press_left;
	bool mouse_press_right;
	int scene_width;
	int scene_height;
	int blur_time;
	int blur_type;
	bool head_rotate_on;
	bool light_rotate_on;
	int gaussion_type;
	float toadd_weight;

	ShaderProgram* shader_bling;
	ShaderProgram* shader_light;
	ShaderProgram* shader_test;
	ShaderProgram* shader_depth;
	ShaderProgram* shader_blur;
	ShaderProgram* shader_add;
	ShaderProgram* shader_thickness;
	ShaderProgram* shader_shadow;

	GLuint shader_bling_ID;
	GLuint shader_light_ID;
	GLuint shader_test_ID;
	GLuint shader_depth_ID;
	GLuint shader_blur_ID;
	GLuint shader_add_ID;
	GLuint shader_thickness_ID;
	GLuint shader_shadow_ID;

	const double blur_weight[5] = { 0.07, 0.18, 0.21, 0.29, 1 };
};

