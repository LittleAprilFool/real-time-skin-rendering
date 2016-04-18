#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::LoadTexture(int gl_texture_para,int texture_ID, char* file_name) {
	glActiveTexture(gl_texture_para);
	TextureManager::Inst()->LoadTexture(file_name, texture_ID, GL_BGR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // ÏßÐÔÂË²¨
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
