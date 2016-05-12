#pragma once
#include "stdafx.h"
class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	GLuint InitShader(const char* vertex_shader_file, const char* fragment_shader_file);
};
