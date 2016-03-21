#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace glm;

typedef std::vector<int> face;
typedef std::vector<vec3> v_n;
typedef std::vector<face> group;

class obj3d
{
public:
	obj3d();
	~obj3d();
	std::vector<vec3> vertice;
	std::vector<vec3> vnormal;
	std::vector<vec2> texture;
	std::vector<group> vgroup;
	std::vector<group> tgroup;

	void loadMesh(char* filename);
	void clear();
	int faceNum;

private:
	std::vector<v_n> vvn;
};

