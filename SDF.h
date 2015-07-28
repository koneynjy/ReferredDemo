#ifndef _SDF_H
#define _SDF_H
//#
#include "SDF/Config.h"
#include "GeometryGenerator.h"
struct SDFModel
{
	MeshData *meshData;
	FDistanceFieldVolumeData *sdfData;
	FBoxSphereBounds *boxSphereBounds;
	SDFModel(){};
	SDFModel(GeometryGenerator::MeshData &md);
	void GenerateSDF(
		float DistanceFieldResolutionScale, 
		bool bGenerateAsIfTwoSided
		);

	void GetSDFData(SDFFloat* data, uint32&w, uint32&h, uint32&d);
	~SDFModel();
};


#endif // !_SDF_H
