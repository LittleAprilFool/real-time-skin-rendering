#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
#include "Shader.h"

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
	void BufferObjectData();
	void Clear();
	int face_number;

	GLuint vao_handles;

protected:
	void ComputeVnormal();
	void ComputeTBN();

private:
	void PrintVertice();
	void PrintVnormal();
	void PrintTexture();
	void PrintT();
	void PrintB();
};

