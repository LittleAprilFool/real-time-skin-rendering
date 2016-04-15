#include "stdafx.h"
#include "TestScene.h"

Object test_head;

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

void TestScene::InitScene()
{
	InitGLFunc_();
	InitParameters_();

	test_head.LoadMesh("cube.obj");
	test_head.AttachShader("vbling2.glsl", "fbling2.glsl");
	test_head.BufferObjectData();
	GetUniformLocations_(test_head.shader_ID);
}

void TestScene::RenderScene()
{
	UpdateMatrix_();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	TransferDataToShader_();
	glDrawArrays(GL_QUADS, 0, test_head.face_number * 4);
	//glDrawArrays(GL_LINES, 0, test_head.face_number * 4);
}

void TestScene::KeyboardFunction(int key, int action)
{
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) eye = vec3(0, 0, 1);
	if (key == GLFW_KEY_X && action == GLFW_PRESS) eye = vec3(1, 0, 0);
	if (key == GLFW_KEY_Y && action == GLFW_PRESS) eye = vec3(-1, 0, 0);
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) rotate_factor.y += 0.2;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) rotate_factor.y -= 0.2;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) rotate_factor.x += 0.2;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) rotate_factor.x -= 0.2;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) InitParameters_();
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) scale_factor += 0.1;
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) scale_factor -= 0.1;
	if (key == GLFW_KEY_P && action == GLFW_PRESS) PrintViewMatrix_();
}

void TestScene::InitGLFunc_()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void TestScene::InitParameters_()
{
	scale_factor = 1;
	rotate_factor = vec3(0);

	GLfloat  iLeft = -1;
	GLfloat iRight = 1;
	GLfloat iBottom = -1;
	GLfloat iTop = 1;
	GLfloat  zNear = -1.0;
	GLfloat zFar = 1.0;
	projection_matrix = ortho(iLeft, iRight, iBottom, iTop, zNear, zFar);

	eye = vec3(1.0, 0.0, 0.0);
	at = vec3(0.0, 0.0, 0.0);
	up = vec3(0.0, 1.0, 0.0);
	view_matrix = lookAt(eye, at, up);
	//view_matrix = lookAt(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0));
	model_matrix = mat4(1.0);
}

void TestScene::GetUniformLocations_(GLuint shader_ID)
{
	loc_model = glGetUniformLocation(shader_ID, "model_matrix");
	loc_view = glGetUniformLocation(shader_ID, "view_matrix");
	loc_projection = glGetUniformLocation(shader_ID, "projection_matrix");
}

void TestScene::UpdateMatrix_()
{
	model_matrix = mat4(1.0);
	model_matrix = rotate<float>(model_matrix, rotate_factor.y, vec3(0, 1, 0));
	model_matrix = rotate<float>(model_matrix, rotate_factor.x, vec3(1, 0, 0));
	model_matrix = scale(model_matrix, vec3(scale_factor));
	view_matrix = lookAt(eye, at, up);
}

void TestScene::TransferDataToShader_()
{
	glUniformMatrix4fv(loc_model, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(loc_view, 1, GL_TRUE, &view_matrix[0][0]);
	glUniformMatrix4fv(loc_projection, 1, GL_TRUE, &projection_matrix[0][0]);
}

void TestScene::PrintViewMatrix_()
{
	for (int i = 0; i < 4; i++)
		std::cout << view_matrix[i].r << " " << view_matrix[i].g << " " << view_matrix[i].b << " " << view_matrix[i].a << std::endl;

}
