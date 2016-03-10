#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;
using namespace glm;

typedef vector<int> face;

typedef vector<face> group;

class obj3d
{
public:
	obj3d();
	~obj3d();
	vector<vec3> vertice;
	vector<vec2> texture;
	vector<group> vgroup;
	vector<group> tgroup;
	void loadMesh(char* filename);
	void clear();
	int faceNum;

private:
};

