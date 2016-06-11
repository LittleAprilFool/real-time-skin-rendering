#pragma once
#include "Scene.h"
class SkyBox : 
	public Scene
{
public:
	SkyBox();
	void InitScene(int width, int height);
	void RenderScene();
	void LoadSkyBox();
	void KeyboardFunction(int key, int action);
	void UpdateMVP();
	void UpdateEye(vec3 eye_pos);
	~SkyBox();
	Object* box;
	GLuint shader_box_ID;

private:
	void GetUniformLocations_();
	GLuint texture_skybox_ID;
	ShaderProgram* shader_box;
	GLuint loc_map_sky;
	GLuint loc_model;
	GLuint loc_view;
	GLuint loc_projection;
	vec3 eye;
	vec3 at;
	vec3 up;
	mat4 projection_matrix;
	mat4 model_matrix;
	mat4 view_matrix;
};

