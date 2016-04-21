#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::LoadTexture(int gl_texture_para, char* file_name) {
	glActiveTexture(gl_texture_para);
	TextureManager::Inst()->LoadTexture(file_name, gl_texture_para - GL_TEXTURE0, GL_BGR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // ÏßÐÔÂË²¨
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint Scene::LoadShader(ShaderProgram* shader, const char* vertex_shader_filename, const char* fragment_shader_filename) {
	shader = new ShaderProgram;
	GLuint shader_ID;
	shader_ID = shader->InitShader(vertex_shader_filename, fragment_shader_filename);
	return shader_ID;
}

//this function is for creating normal color mode for rendering screen to texture
GLuint Scene::CreateRenderTexture_(int texture_unit, int width, int height)
{
	//create FBO
	GLuint FBOName;
	glGenFramebuffers(1, &FBOName);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOName);

	glActiveTexture(texture_unit);

	GLuint texture_ID = texture_unit - GL_TEXTURE0;
	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);

	//bind to shader
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_ID, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	//glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FALSE in creating rendered texture!!" << std::endl;

	return FBOName;
}


//this function is for creating depth color mode for rendering screen to texture
GLuint Scene::CreateRenderTextureForShadow_(int texture_unit, int width, int height)
{
	//create FBO
	GLuint FBOName;
	glGenFramebuffers(1, &FBOName);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOName);

	glActiveTexture(texture_unit);

	GLuint texture_ID;
	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	std::cout << texture_ID << std::endl;

	//bind to shader
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_ID, 0);

	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FALSE in creating rendered texture!!" << std::endl;

	return FBOName;
}