#include "stdafx.h"
#include "myscene.h"

Object head;

HeadScene::HeadScene()
{
}


HeadScene::~HeadScene()
{
}

void HeadScene::RenderScene() 
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vec3 eye(1.0, 0.0, 0.0);
	vec3 at(0.0, 0.0, 0.0);
	vec3 up(0.0, 1.0, 0.0);
	mat4 mv = lookAt(eye, at, up);
	mv = scale(mv, vec3(scale_factor));
	mv = rotate<float>(mv, rotate_factor.y, vec3(0, 1, 0));
	mv = rotate<float>(mv, rotate_factor.x, vec3(1, 0, 0));
	glUniformMatrix4fv(loc_model_view, 1, GL_TRUE, &mv[0][0]);

	GLfloat  iLeft = -0.2;
	GLfloat iRight = 0.2;
	GLfloat iBottom = -0.2;
	GLfloat iTop = 0.2;
	GLfloat  zNear = -5;
	GLfloat zFar = 1;
	mat4  p = ortho(iLeft, iRight, iBottom, iTop, zNear, zFar);
	glUniformMatrix4fv(loc_projection, 1, GL_TRUE, &p[0][0]);

	glUniform1f(loc_translucency, translucency_value);
	glUniform1f(loc_mode, shading_mode);

	glUniform3f(loc_light_pos, light_position.x, light_position.y, light_position.z);
	glUniform3f(loc_light_color, light_color.x, light_color.y, light_color.z);
	glUniform3f(loc_Kd, Kd.x, Kd.y, Kd.z);
	glUniform3f(loc_global_ambient, global_ambient.x, global_ambient.y, global_ambient.z);

	if (display_mode == 1) glDrawArrays(GL_QUADS, 0, head.face_number * 4);
	if (display_mode == 2) glDrawArrays(GL_POINTS, 0, head.face_number * 4);
	if (display_mode == 3) glDrawArrays(GL_LINES, 0, head.face_number * 4);

}

void HeadScene::InitScene() 
{
	InitParameters_();
	head.LoadMesh("head.obj");
	head.AttachShader("vbling.glsl", "fbling.glsl");
	head.BufferObjectData();
	GetUniformLocations_();

	LoadTexture(GL_TEXTURE0, texture_kd_ID, "head-texture.jpg");
	LoadTexture(GL_TEXTURE1, texture_bump_ID, "head-normal.jpg");

}

void HeadScene::KeyboardFunction(int key, int action) 
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) light_position.y += 50;
	if (key == GLFW_KEY_S && action == GLFW_PRESS) light_position.y -= 50;
	if (key == GLFW_KEY_A && action == GLFW_PRESS) light_position.x += 50;
	if (key == GLFW_KEY_D && action == GLFW_PRESS) light_position.x -= 50;
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) light_position.z += 50;
	if (key == GLFW_KEY_E && action == GLFW_PRESS) light_position.z -= 50;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) rotate_factor.y += 0.2;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) rotate_factor.y -= 0.2;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) rotate_factor.x += 0.2;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) rotate_factor.x -= 0.2;

	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) display_mode = 1;
	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) display_mode = 2;
	if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS) display_mode = 3;
	if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS) shading_mode = 1;
	if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS) shading_mode = 2;
	if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS) shading_mode = 3;
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) scale_factor += 0.1;
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) scale_factor -= 0.1;
}

void HeadScene::InitParameters_()
{
	scale_factor = 1;
	rotate_factor = vec3(0);
	translucency_value = 0.5;
	shading_mode = 1;
	display_mode = 1;
	light_position = vec3(100, 100, 100);
	light_color = vec3(1, 1, 1);
	global_ambient = vec3(0.5, 0.5, 0.5);
	Kd = vec3(0.5, 0.5, 0.5);
	texture_kd_ID = 0;
	texture_bump_ID = 1;
}

void HeadScene::GetUniformLocations_()
{
	loc_model_view = glGetUniformLocation(head.shader_ID, "model_view");
	loc_projection = glGetUniformLocation(head.shader_ID, "projection");
	loc_translucency = glGetUniformLocation(head.shader_ID, "translucency");
	loc_map_kd = glGetUniformLocation(head.shader_ID, "map_kd");
	loc_map_bump = glGetUniformLocation(head.shader_ID, "map_bump");
	loc_light_pos = glGetUniformLocation(head.shader_ID, "Light.position");
	loc_light_color = glGetUniformLocation(head.shader_ID, "Light.color");
	loc_Kd = glGetUniformLocation(head.shader_ID, "Kd");
	loc_global_ambient = glGetUniformLocation(head.shader_ID, "global_ambient");
	loc_mode = glGetUniformLocation(head.shader_ID, "mode");
}