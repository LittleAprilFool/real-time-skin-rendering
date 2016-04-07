#include "stdafx.h"
#include "myobject.h"


Head::Head()
{
	InitParameters_();
}


Head::~Head()
{
}

void Head::InitBuffer() 
{
	BufferObjectData();

	loc_model_view = glGetUniformLocation(shader_ID, "model_view");
	loc_projection = glGetUniformLocation(shader_ID, "projection");
	loc_translucency = glGetUniformLocation(shader_ID, "translucency");
	loc_map_kd = glGetUniformLocation(shader_ID, "map_kd");
	loc_map_bump = glGetUniformLocation(shader_ID, "map_bump");
	loc_light_pos = glGetUniformLocation(shader_ID, "Light.position");
	loc_light_color = glGetUniformLocation(shader_ID, "Light.color");
	loc_Kd = glGetUniformLocation(shader_ID, "Kd");
	loc_global_ambient = glGetUniformLocation(shader_ID, "global_ambient");
	loc_mode = glGetUniformLocation(shader_ID, "mode");
}

void Head::InitParameters_() 
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
}

void Head::RenderObject() 
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

	if (display_mode == 1) glDrawArrays(GL_QUADS, 0, face_number * 4);
	if (display_mode == 2) glDrawArrays(GL_POINTS, 0, face_number * 4);
	if (display_mode == 3) glDrawArrays(GL_LINES, 0, face_number * 4);
}

Cube::Cube() 
{
}

Cube::~Cube()
{
}

void Cube::InitBuffer()
{
	BufferObjectData();
}

void Cube::RenderObject()
{
}