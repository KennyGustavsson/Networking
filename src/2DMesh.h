#pragma once
using namespace std;

#include "vector2.h"
#include <vector>

#include "iostream"
#include <fstream>
#include <strstream>
#include <string>  

struct Triangle
{
	Triangle() {}

	Triangle(vector2 p1, vector2 p2, vector2 p3)
	{
		point[0] = p1;
		point[1] = p2;
		point[2] = p3;
	}

	vector2 point[3];
};

struct Mesh
{
	std::vector<Triangle> t = std::vector<Triangle>();

	bool LoadFromObjFile(string sFilename)
	{
		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		t.clear();

		vector<vector2> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				if (line[1] == 't')
				{

				}
				else
				{
					vector2 v;
					s >> junk >> v.x >> v.y;
					verts.push_back(v);
				}
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				t.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}
		return true;
	}

	void Scale(float f)
	{
		for (int i = 0; i < t.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				t[i].point[j].x *= f;
				t[i].point[j].y *= f;
			}
		}
	}
};