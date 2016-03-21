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
	std::ifstream infile(filename);
	std::string line;
	group ivgroup;
	group itgroup;
	faceNum = 0;
	//read obj file line by line
	while (std::getline(infile, line))
	{
		std::istringstream ss(line);
		std::string sub;
		ss >> sub;

		//vertices
		if (sub.compare("v") == 0)
		{
			ss >> sub; float x = stof(sub);
			ss >> sub; float y = stof(sub);
			ss >> sub; float z = stof(sub);
			vec3 point(x, y, z);
			vertice.push_back(point);
			v_n nvn;
			vvn.push_back(nvn);
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
				std::string left = sub.substr(0, pos);
				std::string right = sub.substr(pos + 1);
				int ileft = stoi(left) - 1;
				int iright = stoi(right) - 1;
				vface.push_back(ileft);
				tface.push_back(iright);
			}
			ivgroup.push_back(vface);
			itgroup.push_back(tface);

			//compute face normal
			vec3 p1 = vertice[vface[0]] - vertice[vface[1]];
			vec3 p2 = vertice[vface[1]] - vertice[vface[2]];
			vec3 vn = cross(p1, p2);
			normalize(vn);
			for (int i = 0; i < 4; i++)
			{
				vvn[vface[i]].push_back(vn);
			}
			faceNum++;
		}
	}
	vgroup.push_back(ivgroup);
	tgroup.push_back(itgroup);

	for (int i = 0; i < vvn.size(); i++)
	{
		vec3 sum;
		for (int j = 0; j < vvn[i].size(); j++) {
			sum = sum + vvn[i][j];
		}
		normalize(sum);
		vnormal.push_back(sum);
	}
}