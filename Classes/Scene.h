#include "Object3d.h"
#include "TextureManager.h"
#include "Shader.h"
#pragma once
class Scene
{
public:
	Scene();
	~Scene();
	virtual void RenderScene() = 0;
	virtual void InitScene(int width, int height) = 0;
	virtual void KeyboardFunction(int key, int action) = 0;

protected:
	void LoadTexture(int gl_texture_para, char* file_name);
	GLuint LoadShader(ShaderProgram* shader, const char* vertex_shader_filename, const char* fragment_shader_filename);
	GLuint CreateRenderTexture_(int texture_unit, int width, int height);
	GLuint CreateRenderTextureForDepth_(int texture_unit, int width, int height);
};

