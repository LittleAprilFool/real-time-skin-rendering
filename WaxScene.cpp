#include "stdafx.h"
#include "WaxScene.h"


WaxScene::WaxScene()
{
}


WaxScene::~WaxScene()
{
}

void WaxScene::RenderScene()
{
	UpdateModelMatrix_();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderObject_(bunny, shader_bling_ID);
}

void WaxScene::RenderLight()
{

}

void WaxScene::InitScene(int width, int height)
{
	InitGLFunc_();
	InitParameters_();

	scene_width = width;
	scene_height = height;

	shader_bling_ID = LoadShader(shader_bling, "./Shader/vbling.glsl", "./Shader/fbling.glsl");
	bunny = new Object;
	bunny->LoadMesh("./Resources/bunny.obj", 0);
	bunny->BufferObjectData();
}

void WaxScene::RenderObject_(Object* obj, GLuint shader_ID)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(obj->vao_handles);
	glUseProgram(shader_ID);
	GetUniformLocations_(shader_ID);
	TransferDataToShader_();
	DrawArray_(obj->face_number);
}

void WaxScene::DrawArray_(int face_number)
{
	if (display_mode == 1) glDrawArrays(GL_TRIANGLES, 0, face_number * 3);
	if (display_mode == 2) glDrawArrays(GL_POINTS, 0, face_number * 3);
	if (display_mode == 3) glDrawArrays(GL_LINES, 0, face_number * 3);
}

void WaxScene::TransferDataToShader_()
{
	glUniformMatrix4fv(loc_model, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(loc_view, 1, GL_TRUE, &view_matrix[0][0]);
	glUniformMatrix4fv(loc_projection, 1, GL_TRUE, &projection_matrix[0][0]);

	glUniform1f(loc_mode, shading_mode);
	glUniform1i(loc_blur_type, blur_type);
	glUniform1i(loc_gaussion_type, gaussion_type);

	glUniform3f(loc_light_pos, light_position.x, light_position.y, light_position.z);
	glUniform3f(loc_light_la, light_la.x, light_la.y, light_la.z);
	glUniform3f(loc_light_ld, light_ld.x, light_ld.y, light_ld.z);
	glUniform3f(loc_light_ls, light_ls.x, light_ls.y, light_ls.z);
	glUniform3f(loc_material_ka, material_ka.x, material_ka.y, material_ka.z);
	glUniform3f(loc_material_kd, material_kd.x, material_kd.y, material_kd.z);
	glUniform3f(loc_material_ks, material_ks.x, material_ks.y, material_ks.z);
	glUniform1f(loc_material_shininess, material_shininess);
	glUniform3f(loc_eye_pos, eye.x, eye.y, eye.z);

	glUniformMatrix4fv(loc_depth_model_matrix, 1, GL_TRUE, &depth_model_matrix[0][0]);
	glUniformMatrix4fv(loc_depth_view_matrix, 1, GL_TRUE, &depth_view_matrix[0][0]);
	glUniformMatrix4fv(loc_depth_projection_matrix, 1, GL_TRUE, &depth_projection_matrix[0][0]);
}

void WaxScene::GetUniformLocations_(GLuint shader_ID)
{
	loc_model = glGetUniformLocation(shader_ID, "model_matrix");
	loc_view = glGetUniformLocation(shader_ID, "view_matrix");
	loc_projection = glGetUniformLocation(shader_ID, "projection_matrix");

	loc_light_pos = glGetUniformLocation(shader_ID, "Light.position");
	loc_light_la = glGetUniformLocation(shader_ID, "Light.la");
	loc_light_ld = glGetUniformLocation(shader_ID, "Light.ld");
	loc_light_ls = glGetUniformLocation(shader_ID, "Light.ls");

	loc_material_ka = glGetUniformLocation(shader_ID, "Material.ka");
	loc_material_kd = glGetUniformLocation(shader_ID, "Material.kd");
	loc_material_ks = glGetUniformLocation(shader_ID, "Material.ks");
	loc_material_shininess = glGetUniformLocation(shader_ID, "Material.shininess");

	loc_mode = glGetUniformLocation(shader_ID, "mode");
	loc_eye_pos = glGetUniformLocation(shader_ID, "eye_position");
	loc_blur_type = glGetUniformLocation(shader_ID, "blur_type");
	loc_gaussion_type = glGetUniformLocation(shader_ID, "gaussion_type");

	loc_map_kd = glGetUniformLocation(shader_ID, "map_kd");
	loc_map_bump = glGetUniformLocation(shader_ID, "map_bump");
	loc_map_depth = glGetUniformLocation(shader_ID, "map_depth");
	loc_map_scattered = glGetUniformLocation(shader_ID, "map_scattered");
	loc_map_light = glGetUniformLocation(shader_ID, "map_light");
	loc_map_blur = glGetUniformLocation(shader_ID, "map_blur");
	loc_map_beckmann = glGetUniformLocation(shader_ID, "map_beckmann");
	loc_map_toblur = glGetUniformLocation(shader_ID, "map_toblur");
	loc_map_add = glGetUniformLocation(shader_ID, "map_add");
	loc_map_toadd = glGetUniformLocation(shader_ID, "map_toadd");
	loc_map_afteradd = glGetUniformLocation(shader_ID, "map_afteradd");

	loc_depth_model_matrix = glGetUniformLocation(shader_ID, "depth_model_matrix");
	loc_depth_view_matrix = glGetUniformLocation(shader_ID, "depth_view_matrix");
	loc_depth_projection_matrix = glGetUniformLocation(shader_ID, "depth_projection_matrix");
}

void WaxScene::InitGLFunc_()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void WaxScene::InitParameters_()
{
	scale_factor = 1;
	rotate_factor = vec3(0);

	shading_mode = 1;
	display_mode = 1;

	GLfloat iLeft = -0.2;
	GLfloat iRight = 0.2;
	GLfloat iBottom = -0.2;
	GLfloat iTop = 0.2;
	GLfloat zNear = 0;
	GLfloat zFar = 1;

	projection_matrix = ortho(iLeft, iRight, iBottom, iTop, zNear, zFar);

	eye = vec3(0, 0, 1);
	at = vec3(0, 0, 0);
	up = vec3(0, 1, 0);
	view_matrix = lookAt(eye, at, up);
	model_matrix = mat4(1);

	light_position = vec3(0, 0, 1);
	light_la = vec3(0.5, 0.5, 0.5);
	light_ld = vec3(1, 1, 1);
	light_ls = vec3(0.3, 0.3, 0.3);

	material_ka = vec3(0.5, 0.5, 0.5);
	material_kd = vec3(0.5, 0.5, 0.5);
	material_ks = vec3(0.2, 0.2, 0.2);
	material_shininess = 1;

	mouse_position = vec2(0, 0);
	mouse_position_o = vec2(0, 0);

	mouse_press_left = false;
	mouse_press_right = false;
}

void WaxScene::KeyboardFunction(int key, int action)
{

}

void WaxScene::MouseControl(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouse_press_left = true;
		mouse_position_o = mouse_position;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mouse_press_left = false;
		mouse_position_o = vec2(0, 0);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mouse_press_right = true;
		mouse_position_o = mouse_position;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		mouse_press_right = false;
		mouse_position_o = vec2(0, 0);
	}
}

void WaxScene::CursorPosition(double xpos, double ypos)
{
	mouse_position.x = xpos;
	mouse_position.y = ypos;

	if (mouse_press_left == true)
	{
		rotate_factor.y = rotate_factor.y + (mouse_position.x - mouse_position_o.x) / (10 * scene_width);
		rotate_factor.x = rotate_factor.x - (mouse_position.y - mouse_position_o.y) / (10 * scene_height);
	}

	if (mouse_press_right == true)
	{
		scale_factor = scale_factor + (mouse_position.y - mouse_position_o.y + mouse_position.x - mouse_position_o.x) / (20 * scene_width);
	}
}

void WaxScene::UpdateModelMatrix_()
{
	model_matrix = mat4(1);
	model_matrix = scale(model_matrix, vec3(scale_factor));
	model_matrix = rotate<float>(model_matrix, rotate_factor.y, vec3(0, 1, 0));
	model_matrix = rotate<float>(model_matrix, rotate_factor.x, vec3(1, 0, 0));
	
	view_matrix = lookAt(eye, at, up);

	depth_projection_matrix = projection_matrix;
	depth_view_matrix = lookAt(light_position, at, up);
	depth_model_matrix = model_matrix;
}
