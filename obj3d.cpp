#include "stdafx.h"
#include "obj3d.h"

obj3d::obj3d()
{
}


obj3d::~obj3d()
{
}

void obj3d::clear() {
	vertice.clear();
	texture.clear();
	vgroup.clear();
	faceNum = 0;
}

void obj3d::loadMesh(char* filename) {
	ifstream infile(filename);
	string line;
	group ivgroup;
	group itgroup;
	faceNum = 0;
	//read obj file line by line
	while (getline(infile, line))
	{
		istringstream ss(line);
		string sub;
		ss >> sub;

		//vertices
		if (sub.compare("v") == 0)
		{
			ss >> sub; float x = stof(sub);
			ss >> sub; float y = stof(sub);
			ss >> sub; float z = stof(sub);
			vec3 point(x, y, z);
			vertice.push_back(point);
			//cout << point.x << point.y << point.z << endl;
		}

		//vtexture
		if (sub.compare("vt") == 0)
		{
			ss >> sub; float u = stof(sub);
			ss >> sub; float v = stof(sub);
			vec2 uv(u, v);
			texture.push_back(uv);
		}

		//group
		if (sub.compare("g") == 0)
		{
			if (faceNum != 0) {
				vgroup.push_back(ivgroup);
				tgroup.push_back(itgroup);
			}
			ivgroup.clear();
			itgroup.clear();
		}

		if (sub.compare("f") == 0)
		{
			face vface;
			face tface;
			for (int i = 0; i < 4; i++) {
				ss >> sub;
				int pos = sub.find('/');
				string left = sub.substr(0, pos);
				string right = sub.substr(pos + 1);
				int ileft = stoi(left);
				int iright = stoi(right);
				vface.push_back(ileft);
				tface.push_back(iright);
			}
			ivgroup.push_back(vface);
			itgroup.push_back(tface);
			faceNum++;
		}
	}
	vgroup.push_back(ivgroup);
	tgroup.push_back(itgroup);
}