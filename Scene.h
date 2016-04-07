#include "obj3d.h"
#include "TextureManager.h"
#pragma once
class Scene
{
public:
	Scene();
	~Scene();
	virtual void RenderScene() = 0;
	virtual void InitScene() = 0;
	virtual void KeyboardFunction(int key, int action) = 0;

protected:
	void LoadTexture(int gl_texture_para, int texture_ID, char* file_name);

};

