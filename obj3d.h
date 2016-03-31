#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace glm;

typedef std::vector<int> face;
typedef std::vector<face> group;

class obj3d
{
public:
	obj3d();
	~obj3d();
	std::vector<vec3> vertice;
	std::vector<vec3> vnormal;
	std::vector<vec4> T;
	std::vector<vec3> N;
	std::vector<vec3> B;
	std::vector<vec2> texture;
	std::vector<group> vgroup;
	std::vector<group> tgroup;

	void loadMesh(char* filename);
	void clear();
	int faceNum;

private:
	void compute_vnormal();
	void compute_tbn();
	void print_vertice();
	void print_vnormal();
	void print_texture();
	void print_T();
	void print_B();
};

