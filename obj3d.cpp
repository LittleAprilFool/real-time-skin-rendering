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
			faceNum++;
		}
	}
	vgroup.push_back(ivgroup);
	tgroup.push_back(itgroup);

	compute_vnormal();
	compute_tbn();
	//print_vertice();
	//print_vnormal();
	//print_texture();
	//print_T();
	//print_B();
}

void obj3d::compute_vnormal() {
	std::vector<std::vector<vec3> > vvn;
	for (auto i = vertice.cbegin(); i!= vertice.cend(); i++)
	{
		std::vector<vec3> tmp;
		vvn.push_back(tmp);
	}

	for (auto gr = vgroup.cbegin(); gr != vgroup.cend(); gr ++)
	{
		for (auto fc = gr->begin(); fc != gr->end(); fc ++) 
		{
			//for each face, compute normal
			vec3 p1 = vertice[fc->at(0)] - vertice[fc->at(1)];
			vec3 p2 = vertice[fc->at(1)] - vertice[fc->at(2)];
			vec3 vn = normalize(cross(p1, p2));
			for (auto p = fc->begin(); p != fc->end(); p++)
			{
				vvn[*p].push_back(vn);
			}
		}
	}

	for (auto p = vvn.cbegin(); p != vvn.cend(); p ++) 
	{
		vec3 sum;
		for (auto vn = p->cbegin(); vn != p->cend(); vn++) 
		{
			sum = sum + *vn;
		}
		vnormal.push_back(normalize(sum));
	}
}


//get T
void obj3d::compute_tbn() {
	std::vector<std::vector<vec3> > vt;
	std::vector<std::vector<vec3> > vn;
	std::vector<std::vector<vec3> > vb;
	for (auto i = vertice.cbegin(); i != vertice.cend(); i++)
	{
		std::vector<vec3> tmp;
		vt.push_back(tmp);
		vn.push_back(tmp);
		vb.push_back(tmp);
	}

	for (int i = 0; i != vgroup.size(); i ++)
	{
		group vg = vgroup[i];
		group tg = tgroup[i];
		int nf = 0;
		for (int j = 0; j != vg.size(); j++)
		{
			face vfc = vg[j];
			face tfc = tg[j];
			nf++;

			vec3 v1 = vertice[vfc[0]];
			vec3 v2 = vertice[vfc[1]];
			vec3 v3 = vertice[vfc[2]];
		
			vec2 w1 = texture[tfc[0]];
			vec2 w2 = texture[tfc[1]];
			vec2 w3 = texture[tfc[2]];
			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r;
			if ((s1*t2 - s2*t1) == 0) r = 0;
			else r = 1.0F / (s1 * t2 - s2 * t1);
			vec3 t = vec3(
				(t2 * x1 - t1 * x2) * r, 
				(t2 * y1 - t1 * y2) * r, 
				(t2 * z1 - t1 * z2) * r);
			
			vec3 b = vec3(
				(s1 * x2 - s2 * x1) * r,
				(s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r);

			t = normalize(t);
			b = normalize(b);

			vec3 n = normalize(cross(t, b));

			for (auto p = vfc.cbegin(); p != vfc.cend(); p++) 
			{
				if (r != 0) {
					vt[*p].push_back(t);
					vb[*p].push_back(b);
					vn[*p].push_back(n);
				}
			}
		}
	}


	for (auto p = vn.cbegin(); p != vn.cend(); p++)
	{
		vec3 sum;
		for (auto nn = p->cbegin(); nn != p->cend(); nn++)
		{
			sum = sum + *nn;
		}
		N.push_back(normalize(sum));
	}

	for (auto p = vb.cbegin(); p != vb.cend(); p++)
	{
		vec3 sum;
		for (auto bb = p->cbegin(); bb != p->cend(); bb++)
		{
			sum = sum + *bb;
		}
		B.push_back(normalize(sum));
	}

	//// Gram-Schmidt orthogonalize
	//// To transform in the opposite direction 
	//// (from object space to tangent space¡ªwhat we want to do 
	//// to the light direction), we can simply use the inverse 
	//// of this matrix. It is not necessarily true that 
	//// the tangent vectors are perpendicular to each other 
	//// or to the normal vector, so the inverse of this matrix 
	//// is not generally equal to its transpose. It is safe to assume, 
	//// however, that the three vectors will at least be close 
	//// to orthogonal, so using the Gram-Schmidt algorithm 
	//// to orthogonalize them should not cause any 
	//// unacceptable distortions.
	//// Reference: http://www.terathon.com/code/tangent.html
	for (int p = 0; p != vt.size(); p++) 
	{
		//std::cout << "log info of p" << p << std::endl;
		vec3 sum;
		//std::cout << vt[p].size() << std::endl;
		for (auto tt = vt[p].begin(); tt != vt[p].end(); tt++) 
		{
			sum = sum + *tt;
			//std::cout <<"sum:"<<sum.x<<" "<<sum.y<<" "<<sum.z<< std::endl;
		}
		vec3 norm = vnormal[p];
		vec3 tan = normalize(sum);
		vec3 tan2 = B[p];

		//std::cout <<"norm "<< norm.x << " " << norm.y << " " << norm.z << std::endl;
		//std::cout <<"tan  "<<tan.x<<" " << tan.y<<" "<<tan.z<< std::endl;

		vec4 tangent = vec4(normalize(tan - norm * dot(norm, tan)),0);
		
		tangent.w = (dot(cross(norm, tan), tan2) < 0.0F) ? -1.0F : 1.0F;

//		std::cout <<"tangent"<<tangent.r<<" "<< tangent.g<<" "<< tangent.b<<" "<< tangent.a << std::endl;
		
		T.push_back(tangent);
	}
}

void obj3d::print_vertice() {
	std::cout << "this is the vertice vector" << std::endl;
	int num = 0;
	for (auto v = vertice.begin(); v != vertice.end(); v++) {
		num ++;
		std::cout <<num<<":"<<v->x << " " << v->y << " " << v->z << std::endl;
	}
}

void obj3d::print_vnormal() {
	std::cout << "this is the vnormal vector" << std::endl;
	int num = 0;
	for (auto v = vnormal.begin(); v != vnormal.end(); v++) {
		num++;
		std::cout << num << ":" << v->x << " " << v->y << " " << v->z << std::endl;
	}
}

void obj3d::print_texture() {
	std::cout << "this is the texture vector" << std::endl;
	int num = 0;
	for (auto v = texture.begin(); v != texture.end(); v++) {
		num++;
		std::cout << num << ":" << v->x << " " << v->y << std::endl;
	}
}

void obj3d::print_T() {
	std::cout << "this is the T vector" << std::endl;
	int num = 0;
	for (auto v = T.begin(); v != T.end(); v++) {
		num++;
		std::cout << num << ":" << v->x << " " << v->y << " " << v->z <<" "<< v->a<< std::endl;
	}
}

void obj3d::print_B() {
	std::cout << "this is the B vector" << std::endl;
	int num = 0;
	for (auto v = B.begin(); v != B.end(); v++) {
		num++;
		std::cout << num << ":" << v->x << " " << v->y << " " << v->z << std::endl;
	}
}