#include "stdafx.h"
#include "SkyBox.h"


SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::InitScene(int width, int height) 
{
	box = new Object;
	box->LoadMesh("./Resources/head.obj", 1);
	box->BufferObjectData();
	shader_box_ID = LoadShader(shader_box, "./Shader/vbox.glsl", "./Shader/fbox.glsl");
	LoadSkyBox();
	eye = vec3(0, 0, 1);
	at = vec3(0, 0, 0);
	up = vec3(0, 1, 0);
	GLfloat  iLeft = -0.2;
	GLfloat iRight = 0.2;
	GLfloat iBottom = -0.2;
	GLfloat iTop = 0.2;
	GLfloat  zNear = 0;
	GLfloat zFar = 1;
	projection_matrix = ortho(iLeft, iRight, iBottom, iTop, zNear, zFar);
	view_matrix = lookAt(eye, at, up);
	model_matrix = mat4(1.0);
}

void SkyBox::LoadSkyBox() {
	texture_skybox_ID = GL_TEXTURE0;
	LoadTexture(texture_skybox_ID, "./Resources/sky2.jpg");
	GetUniformLocations_();
	glUniform1i(loc_map_sky, texture_skybox_ID - GL_TEXTURE0);
}

void SkyBox::RenderScene() {
	UpdateMVP();
	//glDepthMask(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(box->vao_handles);
	glUseProgram(shader_box_ID);
	glDrawArrays(GL_QUADS, 0, box->face_number * 4);
	//glDepthMask(1);
}


void SkyBox::KeyboardFunction(int key, int action) {
	return;
}

void SkyBox::UpdateEye(vec3 eye_pos) {
	eye = eye_pos;
}

void SkyBox::UpdateMVP() {
	glUniformMatrix4fv(loc_model, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(loc_view, 1, GL_TRUE, &view_matrix[0][0]);
	glUniformMatrix4fv(loc_projection, 1, GL_TRUE, &model_matrix[0][0]);
}

void SkyBox::GetUniformLocations_() {
	loc_model = glGetUniformLocation(shader_box_ID, "model_matrix");
	loc_view = glGetUniformLocation(shader_box_ID, "view_matrix");
	loc_projection = glGetUniformLocation(shader_box_ID, "projection_matrix");
	loc_map_sky = glGetUniformLocation(shader_box_ID, "map_sky");
}