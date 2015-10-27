#include "LoadOBJ.h"


OBJLoader::OBJLoader(){}
OBJLoader::~OBJLoader(){}

bool OBJLoader::Init(string file_name, string dir,
	std::vector<XMFLOAT3>& vertices,
	std::vector<USHORT>& indices)
{
	ifstream cin(file_name);
	string s;
	while (cin >> s)
	{
		if (s == "v")
		{
			XMFLOAT3 p;
			cin >> p.x >> p.y >> p.z;
			vertices.push_back(p);
		}
		else if (s == "f")
		{
			USHORT i0, i1, i2;
			cin >> i0 >>
		}
	}


}