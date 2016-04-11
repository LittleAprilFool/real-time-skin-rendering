#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
#include "shader.h"

using namespace glm;

typedef std::vector<int> face;
typedef std::vector<face> group;

class Object
{
public:
	Object();
	~Object();
	std::vector<vec3> vertice;
	std::vector<vec3> vnormal;
	std::vector<vec4> T;
	std::vector<vec3> N;
	std::vector<vec3> B;
	std::vector<vec2> texture;
	std::vector<group> vgroup;
	std::vector<group> tgroup;

	void LoadMesh(char* filename);
	
	void AttachShadowShader(const char* vertex_shader_filename, const char* fragment_shader_filename);
	void AttachShader(const char* vertex_shader_filename, const char* fragment_shader_filename);
	void BufferObjectData();
	void Clear();
	int face_number;
	GLuint shader_ID;
	GLuint shadow_shader_ID;
	GLuint vao_handles;

protected:
	void ComputeVnormal();
	void ComputeTBN();
	ShaderProgram* shader;
	ShaderProgram* shadow_shader;

private:
	void PrintVertice();
	void PrintVnormal();
	void PrintTexture();
	void PrintT();
	void PrintB();
};

