#ifndef _SDF_H
#define _SDF_H
//#
#include "SDF/MeshUtilities.h"
#include "GeometryGenerator.h"
struct SDFModel
{
	MeshData meshData;
	FDistanceFieldVolumeData sdfData;

	SDFModel(){};
	SDFModel(GeometryGenerator::MeshData &md);
};


#endif // !_SDF_H
