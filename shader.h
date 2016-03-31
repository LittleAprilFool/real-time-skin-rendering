#pragma once
#include "stdafx.h"
class shader
{
public:
	shader();
	~shader();
	GLuint initShader(const char* vShaderFile, const char* fShaderFile);

};

