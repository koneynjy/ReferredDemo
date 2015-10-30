#include "SDF.h"
#include <string>
#include <iostream>
#include <vector>
#include <DirectXTex.h>
#include "io.h"
using std::string;
using std::vector;
using namespace DirectX;
int main()
{
// 	string file_name = "D:/lod/proxy/";
// 	vector<CMesh> meshs;
// 	string path = file_name + "*.*";
// 	_finddata_t file;
// 	long lf;
// 	if ((lf = _findfirst(path.c_str(), &file)) == -1l)//_findfirst返回的是long型; long __cdecl _findfirst(const char *, struct _finddata_t *)
// 		return;
// 	else
// 	{
// 		while (_findnext(lf, &file) == 0)//int __cdecl _findnext(long, struct _finddata_t *);如果找到下个文件的名字成功的话就返回0,否则返回-1
// 		{
// 			if (file.attrib == 0x2020)
// 			{
// 				string name(file.name);
// 				if (name != ".." && name != "." && name.length() > 6 &&
// 					name.substr(name.length() - 6, 6) == ".model")  {
// 					CMesh cmesh;
// 					short x = 0, z = 0;
// 					sscanf(name.substr(0, 4).c_str(), "%x", &x);
// 					sscanf(name.substr(4, 4).c_str(), "%x", &z);
// 					cmesh.Init(file_name + name, "D:/", XMFLOAT3(x * 100 + 50.0, 0, z * 100 + 50.0));
// 					meshs.push_back(std::move(cmesh));
// 				}
// 			}
// 		}
// 	}
// 	_findclose(lf);

	CMesh cmesh;
	cmesh.Init("D:/combineObjPath/combineObjPath/0038fff2o/0038fff2o.obj", "D:/", XMFLOAT3(0, 0, 0));

	SDFModel sdf(cmesh);
	sdf.GenerateSDF(10.0f, false);
	float* testData = NULL;
	unsigned w, h, d;
	sdf.GetSDFData(testData, w, h, d);
	TexMetadata data;
	data.height = h;
	data.width = w;
	data.depth = d;
	Image img;
	SaveToDDSFile()

	std::cout << "load" << std::endl;
	
}
