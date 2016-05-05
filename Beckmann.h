#pragma once
#include "shader.h"
class Beckmann
{
public:
	Beckmann();
	~Beckmann();
	void Precompute(int width, int height);
	void Render();

private:
	void Init();
	GLuint LoadShader(ShaderProgram* shader, const char* vertex_shader_filename, const char* fragment_shader_filename);
	ShaderProgram* shader_beckmann;
	GLuint shader_beckmann_ID;
	GLuint loc_texture;
	GLuint vao_handle;
	GLuint vbo_handle;
	int width;
	int height;
};

