#ifndef _SDF_H
#define _SDF_H
#include "MeshHelper.h"
#include "SDF/MeshUtilities.h"

struct SDFModel
{
	MeshData meshData;
	FDistanceFieldVolumeData sdfData;

	SDFModel(){};
	//SDFModel(MeshData<VertexCompact> &md);
};


#endif // !_SDF_H
