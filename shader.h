#pragma once
class shader
{
public:
	shader();
	~shader();
	char* readShader(const char* filename);
	GLuint initShader(const char* vShaderFile, const char* fShaderFile);

};

