#ifndef _MESHUTILITIES
#define _MESHUTILITIES
#include "../MeshHelper.h"
#include "Config.h"
#include "IntVector.h"
#include "kDop.h"
#include "RandomStream.h"
#include "Float16.h"
#include "GraphicMath.h"
#include "BoxSphereBounds.h"
#include "AsyncWork.h"

struct MeshData
{
	std::vector<FVector> Vertices;
	std::vector<unsigned> Indices;
};


class FDistanceFieldVolumeData
{
public:

	/** Signed distance field volume stored in local space. */
	TArray<FFloat16> DistanceFieldVolume;

	/** Dimensions of DistanceFieldVolume. */
	FIntVector Size;

	/** Local space bounding box of the distance field volume. */
	FBox LocalBoundingBox;

	/** Whether the mesh was closed and therefore a valid distance field was supported. */
	bool bMeshWasClosed;

	/** Whether the distance field was built assuming that every triangle is a frontface. */
	bool bBuiltAsIfTwoSided;

	/** Whether the mesh was a plane with very little extent in Z. */
	bool bMeshWasPlane;

	//FDistanceFieldVolumeTexture VolumeTexture;

	FDistanceFieldVolumeData() :
		Size(FIntVector(0, 0, 0))
		, LocalBoundingBox(0)
		, bMeshWasClosed(true)
		, bBuiltAsIfTwoSided(false)
		, bMeshWasPlane(false)
		//,VolumeTexture(*this)
	{}

	SIZE_T GetResourceSize() const
	{
		return sizeof(*this) + DistanceFieldVolume.capacity();
	}

	SIZE_T GetDistanceFieldVolumeData(FFloat16* data)
	{
		data = DistanceFieldVolume.data();
		return DistanceFieldVolume.size();
	}
};

class FMeshBuildDataProvider
{
public:

	/** Initialization constructor. */
	FMeshBuildDataProvider(
		const TkDOPTree<const FMeshBuildDataProvider, uint32>& InkDopTree) :
		kDopTree(InkDopTree)
	{}

	// kDOP data provider interface.

	FORCEINLINE const TkDOPTree<const FMeshBuildDataProvider, uint32>& GetkDOPTree(void) const
	{
		return kDopTree;
	}

	FORCEINLINE const FMatrix& GetLocalToWorld(void) const
	{
		return FMatrix::Identity;
	}

	FORCEINLINE const FMatrix& GetWorldToLocal(void) const
	{
		return FMatrix::Identity;
	}

	FORCEINLINE FMatrix GetLocalToWorldTransposeAdjoint(void) const
	{
		return FMatrix::Identity;
	}

	FORCEINLINE float GetDeterminant(void) const
	{
		return 1.0f;
	}

private:

	const TkDOPTree<const FMeshBuildDataProvider, uint32>& kDopTree;
};

void GenerateStratifiedUniformHemisphereSamples(int32 NumThetaSteps, int32 NumPhiSteps, FRandomStream& RandomStream, TArray<FVector4>& Samples)
{
	Samples.clear();
	Samples.resize(NumThetaSteps * NumPhiSteps, FVector(0));
	for (int32 ThetaIndex = 0; ThetaIndex < NumThetaSteps; ThetaIndex++)
	{
		for (int32 PhiIndex = 0; PhiIndex < NumPhiSteps; PhiIndex++)
		{
			const float U1 = RandomStream.GetFraction();
			const float U2 = RandomStream.GetFraction();

			const float Fraction1 = (ThetaIndex + U1) / (float)NumThetaSteps;
			const float Fraction2 = (PhiIndex + U2) / (float)NumPhiSteps;

			const float R = FMath::Sqrt(1.0f - Fraction1 * Fraction1);

			const float Phi = 2.0f * (float)PI * Fraction2;
			// Convert to Cartesian
			Samples.push_back(FVector4(FMath::Cos(Phi) * R, FMath::Sin(Phi) * R, Fraction1));
		}
	}
}


class FMeshDistanceFieldAsyncTask
{
public:
	FMeshDistanceFieldAsyncTask(TkDOPTree<const FMeshBuildDataProvider, uint32>* InkDopTree,
		const TArray<FVector4>* InSampleDirections,
		FBox InVolumeBounds,
		FIntVector InVolumeDimensions,
		float InVolumeMaxDistance,
		int32 InZIndex,
		TArray<FFloat16>* DistanceFieldVolume)
		:
		kDopTree(InkDopTree),
		SampleDirections(InSampleDirections),
		VolumeBounds(InVolumeBounds),
		VolumeDimensions(InVolumeDimensions),
		VolumeMaxDistance(InVolumeMaxDistance),
		ZIndex(InZIndex),
		OutDistanceFieldVolume(DistanceFieldVolume),
		bNegativeAtBorder(false)
	{}

	void DoWork();

	bool WasNegativeAtBorder() const
	{
		return bNegativeAtBorder;
	}
private:

	// Readonly inputs
	TkDOPTree<const FMeshBuildDataProvider, uint32>* kDopTree;
	const TArray<FVector4>* SampleDirections;
	FBox VolumeBounds;
	FIntVector VolumeDimensions;
	float VolumeMaxDistance;
	int32 ZIndex;
	bool bNegativeAtBorder;
	// Output
	TArray<FFloat16>* OutDistanceFieldVolume;
};


FBoxSphereBounds GenerateBoxSphereBounds(const MeshData& LODModel);

#pragma region TODO


void GenerateSignedDistanceFieldVolumeData(
	const MeshData& LODModel
	//,const TArray<EBlendMode>& MaterialBlendModes
	, const FBoxSphereBounds& Bounds
	, float DistanceFieldResolutionScale
	, bool bGenerateAsIfTwoSided
	, FDistanceFieldVolumeData& OutData);

#pragma endregion

#endif // !_MESHUTILITIES
