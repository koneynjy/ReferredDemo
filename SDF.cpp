#include "SDF.h"

SDFModel::SDFModel(GeometryGenerator::MeshData &md)
{
	TArray<GeometryGenerator::Vertex> &vert = md.Vertices;
	TArray<UINT> &ind = md.Indices;
	TArray<FVector> &vertices = meshData.Vertices;
	TArray<uint32> &indices = meshData.Indices;
	vertices.resize(vert.size());
	indices.resize(ind.size());
	for (uint32 i = 0; i < vertices.size(); i++)
	{
		vertices[i] = *(FVector*)&vert[i].Position;
	}

	for (uint32 i = 0; i < indices.size(); i++)
	{
		indices[i] = ind[i];
	}
}