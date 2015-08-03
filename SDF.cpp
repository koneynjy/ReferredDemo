#include "SDF.h"
#include "SDF/MeshUtilities.h"

SDFModel::SDFModel(GeometryGenerator::MeshData &md)
{
	TArray<GeometryGenerator::Vertex> &vert = md.Vertices;
	TArray<UINT> &ind = md.Indices;
	meshData = new MeshData();
	TArray<FVector> &vertices = meshData->Vertices;
	TArray<uint32> &indices = meshData->Indices;
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
	boxSphereBounds = new FBoxSphereBounds();
	GenerateBoxSphereBounds(boxSphereBounds, meshData);
	sdfData = new FDistanceFieldVolumeData();
}

SDFModel::~SDFModel()
{
	delete meshData;
	delete sdfData;
	delete boxSphereBounds;
}

void SDFModel::GenerateSDF(float DistanceFieldResolutionScale, bool bGenerateAsIfTwoSided)
{
	GenerateSignedDistanceFieldVolumeData(
		*meshData
		, *boxSphereBounds
		, DistanceFieldResolutionScale
		, bGenerateAsIfTwoSided
		, *sdfData);
}

void SDFModel::GetSDFData(SDFFloat*& data, uint32&w, uint32&h, uint32&d)
{
	sdfData->GetDistanceFieldVolumeData(data);
	w = sdfData->Size.X;
	h = sdfData->Size.Y;
	d = sdfData->Size.Z;
}

XMFLOAT3 SDFModel::GetBounds()
{
	FVector ret = sdfData->LocalBoundingBox.Max - sdfData->LocalBoundingBox.Min;
	return XMFLOAT3(ret.X, ret.Y, ret.Z);
}

float SDFModel::GetRes()
{
	return 0.0f;
}