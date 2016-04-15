#pragma once
#include "Scene.h"
class TestScene :
	public Scene
{
public:
	TestScene();
	~TestScene();
	void InitScene();
	void RenderScene();
	void KeyboardFunction(int key, int action);

private:
	GLuint loc_model;
	GLuint loc_view;
	GLuint loc_projection;
	void InitGLFunc_();
	void InitParameters_();
	void GetUniformLocations_(GLuint shader_ID);
	void TransferDataToShader_();
	void UpdateMatrix_();
	mat4 projection_matrix;
	mat4 model_matrix;
	mat4 view_matrix;
	vec3 eye;
	vec3 at;
	vec3 up;
	float scale_factor;
	vec3 rotate_factor;
	void PrintViewMatrix_();
};

