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
	GetUniformLocations_(head.shader_ID);

	LoadTexture(GL_TEXTURE1, texture_kd_ID, "head-texture.jpg");
	LoadTexture(GL_TEXTURE2, texture_bump_ID, "head-normal.jpg");

	FBO_ID = CreateRenderTexture_();

	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	cube.LoadMesh("cube.obj");
	cube.AttachShader("vtest.glsl", "ftest.glsl");
	cube.BufferObjectData();
	GetUniformLocations_(cube.shader_ID);
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

void HeadScene::GetUniformLocations_(GLuint shader_ID)
{
	loc_model_view = glGetUniformLocation(shader_ID, "model_view");
	loc_projection = glGetUniformLocation(shader_ID, "projection");
	loc_translucency = glGetUniformLocation(shader_ID, "translucency");
	loc_light_pos = glGetUniformLocation(shader_ID, "Light.position");
	loc_light_color = glGetUniformLocation(shader_ID, "Light.color");
	loc_Kd = glGetUniformLocation(shader_ID, "Kd");
	loc_global_ambient = glGetUniformLocation(shader_ID, "global_ambient");
	loc_mode = glGetUniformLocation(shader_ID, "mode");
	
	loc_map_kd = glGetUniformLocation(shader_ID, "map_kd");
	glUniform1i(loc_map_kd, 1);
	loc_map_bump = glGetUniformLocation(shader_ID, "map_bump");
	glUniform1i(loc_map_bump, 2);

	loc_map_rendered = glGetUniformLocation(shader_ID, "map_rendered");
	glUniform1i(loc_map_rendered, 0);
}

void HeadScene::InitFBO_()
{
	//create the shadow map texture
	GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	int smWidth = 1024;
	int smHeight = 1024;
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, smWidth, smHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	//assign the shadow map to texture channel 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthTex);

	//create and set up the FBO
	GLuint shadowFBO;
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

	GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);

	//revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint HeadScene::CreateRenderTexture_()
{
	//create FBO
	GLuint FBOName;
	glGenFramebuffers(1, &FBOName);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOName);

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &texture_rendered_ID);
	glBindTexture(GL_TEXTURE_2D, texture_rendered_ID);

	//assign the shadow map to texture channel2
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, texture_rendered_ID);

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

void HeadScene::PrintLoc_() 
{
	std::cout <<"Start to print location ID" << std::endl;
	std::cout << loc_model_view << std::endl;
	std::cout << loc_projection << std::endl;
	std::cout << loc_translucency<< std::endl;
	std::cout << loc_light_pos<< std::endl;
	std::cout << loc_light_color<< std::endl;
	std::cout << loc_Kd<< std::endl;
	std::cout << loc_global_ambient<< std::endl;
	std::cout << loc_mode<< std::endl;
	std::cout << loc_map_kd<< std::endl;
	std::cout << loc_map_bump<< std::endl;
	std::cout << loc_map_rendered<< std::endl;
	std::cout << "End of printing location ID" << std::endl;

}