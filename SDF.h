#ifndef _SDF_H
#define _SDF_H
//#
#include "SDF/Config.h"
#include "GeometryGenerator.h"
#include "Vertex.h"
struct SDFModel
{
public:
	MeshData *meshData;
	FDistanceFieldVolumeData *sdfData;
	FBoxSphereBounds *boxSphereBounds;
	SDFModel(){};
	SDFModel(GeometryGenerator::MeshData &md);
	SDFModel(std::vector<Vertex::Basic32>& vert, std::vector<UINT> &ind);
	void GenerateSDF(
		float DistanceFieldResolutionScale, 
		bool bGenerateAsIfTwoSided
		);

	void GetSDFData(SDFFloat*& data, uint32&w, uint32&h, uint32&d);
	XMFLOAT3 GetBounds();
	float GetRes();
	static SDFModel Merge(SDFModel& m0, FVector& Pos0, SDFModel& m1, FVector& Pos1);
	~SDFModel();
};


#endif // !_SDF_H
