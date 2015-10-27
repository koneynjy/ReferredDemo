#ifndef _OBJ_H
#define _OBJ_H

#include "MeshGeometry.h"
#include "Vertex.h"
#include <string>
#include <vector>	
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;

class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	bool Init(string file_name, string dir,
		std::vector<XMFLOAT3>& vertices,
		std::vector<USHORT>& indices);

};


#endif