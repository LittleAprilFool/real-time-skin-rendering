#include "stdafx.h"
#include "myscene.h"

Object head;
Object cube;

HeadScene::HeadScene()
{
}


HeadScene::~HeadScene()
{
}

void HeadScene::DrawArray_(int face_number)
{
	if (display_mode == 1) glDrawArrays(GL_QUADS, 0, face_number * 4);
	if (display_mode == 2) glDrawArrays(GL_POINTS, 0, face_number * 4);
	if (display_mode == 3) glDrawArrays(GL_LINES, 0, face_number * 4);
}

void HeadScene::RenderScene() 
{
	UpdateModelMatrix_();
	//draw shadow map first
	
	DrawShadowMap_();
	BindFBOForHead_();
}

void HeadScene::DrawShadowMap_() 
{
	//bind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//bind head vao
	glBindVertexArray(head.vao_handles);
	//use head shader
	glUseProgram(head.shadow_shader_ID);
	//get uniform Locations
	GetUniformLocations_(head.shadow_shader_ID);
	//update data to shader
	TransferDataToShader_();
	PassDepthMVP(head.shadow_shader_ID);
	//draw array
	DrawArray_(head.face_number);
}

void HeadScene::BindFBOForCube_() 
{
	//bind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//bind cube vao
	glBindVertexArray(cube.vao_handles);
	//use cube shader
	glUseProgram(cube.shader_ID);
	//get uniform locations
	GetUniformLocations_(cube.shader_ID);
	//update data to shader
	TransferDataToShader_();
	//depth info
	PassDepthMVP(cube.shader_ID);
	//draw array
	DrawArray_(cube.face_number);
}

void HeadScene::BindFBOForHead_()
{
	//bind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//bind cube vao
	glBindVertexArray(head.vao_handles);
	//use cube shader
	glUseProgram(head.shader_ID);
	//get uniform locations
	GetUniformLocations_(head.shader_ID);
	//update data to shader
	TransferDataToShader_();
	//depth info
	PassDepthMVP(head.shader_ID);
	//draw array
	DrawArray_(head.face_number);
}

void HeadScene::TransferDataToShader_()
{
	glUniformMatrix4fv(loc_model, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(loc_view, 1, GL_TRUE, &view_matrix[0][0]);
	glUniformMatrix4fv(loc_projection, 1, GL_TRUE, &projection_matrix[0][0]);

	glUniform1f(loc_translucency, translucency_value);
	glUniform1f(loc_mode, shading_mode);

	glUniform3f(loc_light_pos, light_position.x, light_position.y, light_position.z);
	glUniform3f(loc_light_la, light_la.x, light_la.y, light_la.z);
	glUniform3f(loc_light_ld, light_ld.x, light_ld.y, light_ld.z);
	glUniform3f(loc_light_ls, light_ls.x, light_ls.y, light_ls.z);
	glUniform3f(loc_material_ka, material_ka.x, material_ka.y, material_ka.z);
	glUniform3f(loc_material_kd, material_kd.x, material_kd.y, material_kd.z);
	glUniform3f(loc_material_ks, material_ks.x, material_ks.y, material_ks.z);
	glUniform1f(loc_material_shininess, material_shininess);
}

void HeadScene::InitScene() 
{
	InitGLFunc_();
	InitParameters_();
	
	head.LoadMesh("head.obj");
	head.AttachShadowShader("vshadow.glsl", "fshadow.glsl");
	head.BufferObjectData();
	head.AttachShader("vbling.glsl", "fbling.glsl");

	FBO_ID = CreateRenderTextureForShadow_();


	LoadTexture(GL_TEXTURE1, texture_kd_ID, "head-texture.jpg");
	LoadTexture(GL_TEXTURE2, texture_bump_ID, "head-normal.jpg");
	LoadTexture(GL_TEXTURE3, texture_scattered_ID, "head-scattered.jpg");

	cube.LoadMesh("cube.obj");
	cube.AttachShader("vtest.glsl", "ftest.glsl");
	cube.BufferObjectData();
}

void HeadScene::KeyboardFunction(int key, int action) 
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) light_position.y += 0.2;
	if (key == GLFW_KEY_S && action == GLFW_PRESS) light_position.y -= 0.2;
	if (key == GLFW_KEY_A && action == GLFW_PRESS) light_position.x += 0.2;
	if (key == GLFW_KEY_D && action == GLFW_PRESS) light_position.x -= 0.2;
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) light_position.z += 0.2;
	if (key == GLFW_KEY_E && action == GLFW_PRESS) light_position.z -= 0.1;
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) eye = vec3(0, 0, 1);
	if (key == GLFW_KEY_X && action == GLFW_PRESS) eye = vec3(1, 0, 0);
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) rotate_factor.y += 0.2;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) rotate_factor.y -= 0.2;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) rotate_factor.x += 0.2;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) rotate_factor.x -= 0.2;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) InitParameters_();

	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) display_mode = 1;
	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS) display_mode = 2;
	if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS) display_mode = 3;
	if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS) shading_mode = 1;
	if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS) shading_mode = 2;
	if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS) shading_mode = 3;
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) scale_factor += 0.1;
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) scale_factor -= 0.1;
}

void HeadScene::InitGLFunc_()
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

void HeadScene::InitParameters_()
{
	scale_factor = 1;
	rotate_factor = vec3(0);
	translucency_value = 0.5;
	shading_mode = 1;
	display_mode = 1;
	texture_kd_ID = 0;
	texture_bump_ID = 1;
	texture_scattered_ID = 2;

	GLfloat  iLeft = -0.2;
	GLfloat iRight = 0.2;
	GLfloat iBottom = -0.2;
	GLfloat iTop = 0.2;
	GLfloat  zNear = 0;
	GLfloat zFar = 1;
	projection_matrix = ortho(iLeft, iRight, iBottom, iTop, zNear, zFar);

	eye = vec3(1.0, 0.0, 0.0);
	at = vec3(0.0, 0.0, 0.0);
	up = vec3(0.0, 1.0, 0.0);
	view_matrix = lookAt(eye, at, up);
	model_matrix = mat4(1.0);
	
	light_position = vec3(-1, 0, 0);
	light_la = vec3(0.5, 0.5, 0.5);
	light_ld = vec3(0.3, 0.3, 0.3);
	light_ls = vec3(0.5, 0.5, 0.5);

	material_ka = vec3(0.5, 0.5, 0.5);
	material_kd = vec3(0.5, 0.5, 0.5);
	material_ks = vec3(0.5, 0.5, 0.5);
	material_shininess = 0.3;
}

void HeadScene::UpdateModelMatrix_() 
{
	model_matrix = mat4(1.0);
	model_matrix = scale(model_matrix, vec3(scale_factor));
	model_matrix = rotate<float>(model_matrix, rotate_factor.y, vec3(0, 1, 0));
	model_matrix = rotate<float>(model_matrix, rotate_factor.x, vec3(1, 0, 0));
	view_matrix = lookAt(eye, at, up);
}

void HeadScene::GetUniformLocations_(GLuint shader_ID)
{
	loc_model = glGetUniformLocation(shader_ID, "model_matrix");
	loc_view = glGetUniformLocation(shader_ID, "view_matrix");
	loc_projection = glGetUniformLocation(shader_ID, "projection_matrix");
	loc_translucency = glGetUniformLocation(shader_ID, "translucency");
	loc_light_pos = glGetUniformLocation(shader_ID, "Light.position");
	loc_light_la = glGetUniformLocation(shader_ID, "Light.la");
	loc_light_ld = glGetUniformLocation(shader_ID, "Light.ld");
	loc_light_ls = glGetUniformLocation(shader_ID, "Light.ls");
	loc_material_ka = glGetUniformLocation(shader_ID, "Material.ka");
	loc_material_kd = glGetUniformLocation(shader_ID, "Material.kd");
	loc_material_ks = glGetUniformLocation(shader_ID, "Material.ks");
	loc_material_shininess = glGetUniformLocation(shader_ID, "Material.shininess");
	loc_mode = glGetUniformLocation(shader_ID, "mode");

	loc_map_kd = glGetUniformLocation(shader_ID, "map_kd");
	glUniform1i(loc_map_kd, 1);
	loc_map_bump = glGetUniformLocation(shader_ID, "map_bump");
	glUniform1i(loc_map_bump, 2);
	loc_map_rendered = glGetUniformLocation(shader_ID, "map_rendered");
	glUniform1i(loc_map_rendered, 0);
	loc_map_scattered = glGetUniformLocation(shader_ID, "map_scattered");
	glUniform1i(loc_map_scattered, 3);
}

//this function is for creating normal color mode for rendering screen to texture
GLuint HeadScene::CreateRenderTexture_()
{
	//create FBO
	GLuint FBOName;
	glGenFramebuffers(1, &FBOName);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOName);

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &texture_rendered_ID);
	glBindTexture(GL_TEXTURE_2D, texture_rendered_ID);

	//bind to shader
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenRenderbuffers(1, &depth_buffer_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_ID);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_rendered_ID, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	//glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FALSE in creating rendered texture!!" << std::endl;

	return FBOName;
}


//this function is for creating depth color mode for rendering screen to texture
GLuint HeadScene::CreateRenderTextureForShadow_()
{
	//create FBO
	GLuint FBOName;
	glGenFramebuffers(1, &FBOName);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOName);

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &texture_depth_ID);
	glBindTexture(GL_TEXTURE_2D, texture_depth_ID);

	//bind to shader
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_depth_ID, 0);

	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FALSE in creating rendered texture!!" << std::endl;

	return FBOName;
}

void HeadScene::PassDepthMVP(int shader_ID)
{
	loc_depth_model_matrix = glGetUniformLocation(shader_ID, "depth_model_matrix");
	loc_depth_view_matrix = glGetUniformLocation(shader_ID, "depth_view_matrix");
	loc_depth_projection_matrix = glGetUniformLocation(shader_ID, "depth_projection_matrix");
	
	depth_projection_matrix = projection_matrix;
	depth_view_matrix = lookAt(light_position, at, up);
	depth_model_matrix = model_matrix;

	glUniformMatrix4fv(loc_depth_model_matrix, 1, GL_TRUE, &depth_model_matrix[0][0]);
	glUniformMatrix4fv(loc_depth_view_matrix, 1, GL_TRUE, &depth_view_matrix[0][0]);
	glUniformMatrix4fv(loc_depth_projection_matrix, 1, GL_TRUE, &depth_projection_matrix[0][0]);
}

void HeadScene::PrintLoc_() 
{
	std::cout <<"Start to print location ID" << std::endl;
	std::cout << loc_model<< std::endl;
	std::cout << loc_view << std::endl;
	std::cout << loc_projection << std::endl;
	std::cout << loc_translucency<< std::endl;
	std::cout << loc_light_pos<< std::endl;
	std::cout << loc_mode<< std::endl;
	std::cout << loc_map_kd<< std::endl;
	std::cout << loc_map_bump<< std::endl;
	std::cout << loc_map_rendered<< std::endl;
	std::cout << "End of printing location ID" << std::endl;

}