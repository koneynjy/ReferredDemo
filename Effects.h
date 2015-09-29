//***************************************************************************************
// Effects.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines lightweight effect wrappers to group an effect and its variables.
// Also defines a static Effects class from which we can access all of our effects.
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H

#include "d3dUtil.h"

#define SSR_EFFECT 0x01

#pragma region Effect
class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

#pragma region BasicEffect
class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::wstring& filename);
	~BasicEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProjTex(CXMMATRIX M)               { WorldViewProjTex->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform(CXMMATRIX M)                { ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)    { ShadowMap->SetResource(tex); }
	void SetSsaoMap(ID3D11ShaderResourceView* tex)      { SsaoMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;

	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech;

	ID3DX11EffectTechnique* Light0TexFogTech;
	ID3DX11EffectTechnique* Light1TexFogTech;
	ID3DX11EffectTechnique* Light2TexFogTech;
	ID3DX11EffectTechnique* Light3TexFogTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectTechnique* Light1ReflectTech;
	ID3DX11EffectTechnique* Light2ReflectTech;
	ID3DX11EffectTechnique* Light3ReflectTech;

	ID3DX11EffectTechnique* Light0TexReflectTech;
	ID3DX11EffectTechnique* Light1TexReflectTech;
	ID3DX11EffectTechnique* Light2TexReflectTech;
	ID3DX11EffectTechnique* Light3TexReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipReflectTech;

	ID3DX11EffectTechnique* Light1FogReflectTech;
	ID3DX11EffectTechnique* Light2FogReflectTech;
	ID3DX11EffectTechnique* Light3FogReflectTech;

	ID3DX11EffectTechnique* Light0TexFogReflectTech;
	ID3DX11EffectTechnique* Light1TexFogReflectTech;
	ID3DX11EffectTechnique* Light2TexFogReflectTech;
	ID3DX11EffectTechnique* Light3TexFogReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProjTex;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectShaderResourceVariable* SsaoMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
};
#pragma endregion

#pragma region NormalMapEffect
class NormalMapEffect : public Effect
{
public:
	NormalMapEffect(ID3D11Device* device, const std::wstring& filename);
	~NormalMapEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProjTex(CXMMATRIX M)               { WorldViewProjTex->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt){ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetShadowTransform(CXMMATRIX M)                { ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }
	void SetSsaoMap(ID3D11ShaderResourceView* tex)      { SsaoMap->SetResource(tex); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)    { ShadowMap->SetResource(tex); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;

	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech;

	ID3DX11EffectTechnique* Light0TexFogTech;
	ID3DX11EffectTechnique* Light1TexFogTech;
	ID3DX11EffectTechnique* Light2TexFogTech;
	ID3DX11EffectTechnique* Light3TexFogTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectTechnique* Light1ReflectTech;
	ID3DX11EffectTechnique* Light2ReflectTech;
	ID3DX11EffectTechnique* Light3ReflectTech;

	ID3DX11EffectTechnique* Light0TexReflectTech;
	ID3DX11EffectTechnique* Light1TexReflectTech;
	ID3DX11EffectTechnique* Light2TexReflectTech;
	ID3DX11EffectTechnique* Light3TexReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipReflectTech;

	ID3DX11EffectTechnique* Light1FogReflectTech;
	ID3DX11EffectTechnique* Light2FogReflectTech;
	ID3DX11EffectTechnique* Light3FogReflectTech;

	ID3DX11EffectTechnique* Light0TexFogReflectTech;
	ID3DX11EffectTechnique* Light1TexFogReflectTech;
	ID3DX11EffectTechnique* Light2TexFogReflectTech;
	ID3DX11EffectTechnique* Light3TexFogReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectTech;

	ID3DX11EffectTechnique* Light1SkinnedTech;
	ID3DX11EffectTechnique* Light2SkinnedTech;
	ID3DX11EffectTechnique* Light3SkinnedTech;

	ID3DX11EffectTechnique* Light0TexSkinnedTech;
	ID3DX11EffectTechnique* Light1TexSkinnedTech;
	ID3DX11EffectTechnique* Light2TexSkinnedTech;
	ID3DX11EffectTechnique* Light3TexSkinnedTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipSkinnedTech;

	ID3DX11EffectTechnique* Light1FogSkinnedTech;
	ID3DX11EffectTechnique* Light2FogSkinnedTech;
	ID3DX11EffectTechnique* Light3FogSkinnedTech;

	ID3DX11EffectTechnique* Light0TexFogSkinnedTech;
	ID3DX11EffectTechnique* Light1TexFogSkinnedTech;
	ID3DX11EffectTechnique* Light2TexFogSkinnedTech;
	ID3DX11EffectTechnique* Light3TexFogSkinnedTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogSkinnedTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogSkinnedTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogSkinnedTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogSkinnedTech;

	ID3DX11EffectTechnique* Light1ReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2ReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3ReflectSkinnedTech;

	ID3DX11EffectTechnique* Light0TexReflectSkinnedTech;
	ID3DX11EffectTechnique* Light1TexReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2TexReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3TexReflectSkinnedTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipReflectSkinnedTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipReflectSkinnedTech;

	ID3DX11EffectTechnique* Light1FogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2FogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3FogReflectSkinnedTech;

	ID3DX11EffectTechnique* Light0TexFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light1TexFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2TexFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3TexFogReflectSkinnedTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectSkinnedTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectSkinnedTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProjTex;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectShaderResourceVariable* SsaoMap;
};
#pragma endregion

#pragma region BuildShadowMapEffect
class BuildShadowMapEffect : public Effect
{
public:
	BuildShadowMapEffect(ID3D11Device* device, const std::wstring& filename);
	~BuildShadowMapEffect();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt){ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	
	void SetHeightScale(float f)                        { HeightScale->SetFloat(f); }
	void SetMaxTessDistance(float f)                    { MaxTessDistance->SetFloat(f); }
	void SetMinTessDistance(float f)                    { MinTessDistance->SetFloat(f); }
	void SetMinTessFactor(float f)                      { MinTessFactor->SetFloat(f); }
	void SetMaxTessFactor(float f)                      { MaxTessFactor->SetFloat(f); }

	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }

	ID3DX11EffectTechnique* BuildShadowMapTech;
	ID3DX11EffectTechnique* BuildShadowMapAlphaClipTech;
	ID3DX11EffectTechnique* BuildShadowMapSkinnedTech;
	ID3DX11EffectTechnique* BuildShadowMapAlphaClipSkinnedTech;
	ID3DX11EffectTechnique* TessBuildShadowMapTech;
	ID3DX11EffectTechnique* TessBuildShadowMapAlphaClipTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectScalarVariable* HeightScale;
	ID3DX11EffectScalarVariable* MaxTessDistance;
	ID3DX11EffectScalarVariable* MinTessDistance;
	ID3DX11EffectScalarVariable* MinTessFactor;
	ID3DX11EffectScalarVariable* MaxTessFactor;
 
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};
#pragma endregion

#pragma region SsaoNormalDepthEffect
class SsaoNormalDepthEffect : public Effect
{
public:
	SsaoNormalDepthEffect(ID3D11Device* device, const std::wstring& filename);
	~SsaoNormalDepthEffect();

	void SetWorldView(CXMMATRIX M)                      { WorldView->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTransposeView(CXMMATRIX M)          { WorldInvTransposeView->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt){ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }

	ID3DX11EffectTechnique* NormalDepthTech;
	ID3DX11EffectTechnique* NormalDepthAlphaClipTech;

	ID3DX11EffectTechnique* NormalDepthSkinnedTech;
	ID3DX11EffectTechnique* NormalDepthAlphaClipSkinnedTech;

	ID3DX11EffectMatrixVariable* WorldView;
	ID3DX11EffectMatrixVariable* WorldInvTransposeView;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* TexTransform;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};
#pragma endregion

#pragma region SsaoEffect
class SsaoEffect : public Effect
{
public:
	SsaoEffect(ID3D11Device* device, const std::wstring& filename);
	~SsaoEffect();

	void SetViewRot(CXMMATRIX M)						   { ViewRot->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetViewToTexSpace(CXMMATRIX M)                    { ViewToTexSpace->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                     { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetOffsetVectors(const XMFLOAT4 v[14])            { OffsetVectors->SetFloatVectorArray(reinterpret_cast<const float*>(v), 0, 14); }
	void SetFrustumCorners(const XMFLOAT4 v[4])            { FrustumCorners->SetFloatVectorArray(reinterpret_cast<const float*>(v), 0, 4); }
	void SetOcclusionRadius(float f)                       { OcclusionRadius->SetFloat(f); }
	void SetOcclusionFadeStart(float f)                    { OcclusionFadeStart->SetFloat(f); }
	void SetOcclusionFadeEnd(float f)                      { OcclusionFadeEnd->SetFloat(f); }
	void SetSurfaceEpsilon(float f)                        { SurfaceEpsilon->SetFloat(f); }

	void SetNormalDepthMap(ID3D11ShaderResourceView* srv)  { NormalDepthMap->SetResource(srv); }
	void SetDepthMap(ID3D11ShaderResourceView* srv)		   { DepthMap->SetResource(srv); }
	void SetGBuffer0(ID3D11ShaderResourceView* srv)		   { GBuffer0->SetResource(srv); }
	void SetRandomVecMap(ID3D11ShaderResourceView* srv)    { RandomVecMap->SetResource(srv); }

	ID3DX11EffectTechnique* SsaoTech;
	ID3DX11EffectTechnique* SsaoDeferred;
	ID3DX11EffectTechnique* SsvoDeferred;

	ID3DX11EffectMatrixVariable* ViewRot;
	ID3DX11EffectMatrixVariable* ViewToTexSpace;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* OffsetVectors;
	ID3DX11EffectVectorVariable* FrustumCorners;
	ID3DX11EffectScalarVariable* OcclusionRadius;
	ID3DX11EffectScalarVariable* OcclusionFadeStart;
	ID3DX11EffectScalarVariable* OcclusionFadeEnd;
	ID3DX11EffectScalarVariable* SurfaceEpsilon;

	ID3DX11EffectShaderResourceVariable* NormalDepthMap;
	ID3DX11EffectShaderResourceVariable* RandomVecMap;
	ID3DX11EffectShaderResourceVariable* DepthMap;
	ID3DX11EffectShaderResourceVariable* GBuffer0;
};
#pragma endregion

#pragma region SsaoBlurEffect
class SsaoBlurEffect : public Effect
{
public:
	SsaoBlurEffect(ID3D11Device* device, const std::wstring& filename);
	~SsaoBlurEffect();

	void SetFarClipDist(float f)						   { FarClipDist->SetFloat(f); }
	void SetTexelWidth(float f)                            { TexelWidth->SetFloat(f); }
	void SetTexelHeight(float f)                           { TexelHeight->SetFloat(f); }

	void SetNormalDepthMap(ID3D11ShaderResourceView* srv)  { NormalDepthMap->SetResource(srv); }
	void SetInputImage(ID3D11ShaderResourceView* srv)      { InputImage->SetResource(srv); }
	void SetGBuffer0(ID3D11ShaderResourceView* srv)        { GBuffer0->SetResource(srv); }
	void SetDepthMap(ID3D11ShaderResourceView* srv)        { DepthMap->SetResource(srv); }

	ID3DX11EffectTechnique* HorzBlurTech;
	ID3DX11EffectTechnique* VertBlurTech;
	ID3DX11EffectTechnique* HorzBlurDeferred;
	ID3DX11EffectTechnique* VertBlurDeferred;

	ID3DX11EffectScalarVariable* FarClipDist;
	ID3DX11EffectScalarVariable* TexelWidth;
	ID3DX11EffectScalarVariable* TexelHeight;

	ID3DX11EffectShaderResourceVariable* NormalDepthMap;
	ID3DX11EffectShaderResourceVariable* InputImage;
	ID3DX11EffectShaderResourceVariable* GBuffer0;
	ID3DX11EffectShaderResourceVariable* DepthMap;
};
#pragma endregion

#pragma region SkyEffect
class SkyEffect : public Effect
{
public:
	SkyEffect(ID3D11Device* device, const std::wstring& filename);
	~SkyEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetCubeMap(ID3D11ShaderResourceView* cubemap)  { CubeMap->SetResource(cubemap); }

	ID3DX11EffectTechnique* SkyTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;

	ID3DX11EffectShaderResourceVariable* CubeMap;
};
#pragma endregion

#pragma region GBufferEffect
class GBufferEffect : public Effect
{
public:
	GBufferEffect(ID3D11Device* device, const std::wstring& filename);
	~GBufferEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetFarClipDist(float f)						{ FarClipDist->SetFloat(f); }
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt){ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }

	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }

	ID3DX11EffectTechnique* GBufferTech;
	ID3DX11EffectTechnique* GBufferTechSkinned;
	ID3DX11EffectTechnique* GBufferTechBase;
	ID3DX11EffectTechnique* GBufferTechBaseReflect;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectScalarVariable* FarClipDist;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	ID3DX11EffectVectorVariable* EyePosW;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
};

#pragma endregion

#pragma region DEFERREDSHADING

class DeferredShadingEffect : public Effect
{
public:
	DeferredShadingEffect(ID3D11Device* device, const std::wstring& filename);
	~DeferredShadingEffect();

	void SetViewInv(CXMMATRIX M)						{ ViewInv->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform(CXMMATRIX M)                { ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetAmb(const XMFLOAT4& v)						{ amb->SetRawValue(&v, 0, sizeof(XMFLOAT4)); }
	void SetSsaoMap(ID3D11ShaderResourceView* tex)      { SsaoMap->SetResource(tex); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)    { ShadowMap->SetResource(tex); }
	void SetDepthMap(ID3D11ShaderResourceView* tex)		{ DepthMap->SetResource(tex); }
	void SetGBuffer0(ID3D11ShaderResourceView* tex)		{ GBuffer0->SetResource(tex); }
	void SetGBuffer1(ID3D11ShaderResourceView* tex)		{ GBuffer1->SetResource(tex); }
	void SetSDFShadow(ID3D11ShaderResourceView* tex)	{ SDFShadow->SetResource(tex); }
	void SetSSRMap(ID3D11ShaderResourceView* tex)		{ SSRMap->SetResource(tex); }

	ID3DX11EffectTechnique* DeferredShadingTech;

	ID3DX11EffectMatrixVariable* ViewInv;
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* amb;

	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectShaderResourceVariable* SsaoMap;
	ID3DX11EffectShaderResourceVariable* DepthMap;
	ID3DX11EffectShaderResourceVariable* GBuffer0;
	ID3DX11EffectShaderResourceVariable* GBuffer1;
	ID3DX11EffectShaderResourceVariable* SDFShadow;
	ID3DX11EffectShaderResourceVariable* SSRMap;
};

#pragma endregion


#pragma region SDFShadow

class SDFShadowEffect : public Effect
{
public :
	SDFShadowEffect(ID3D11Device* device, const std::wstring& filename);
	~SDFShadowEffect();

	void SetViewInv(CXMMATRIX M)						{ ViewInv->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetLightDirs(const XMFLOAT3& v)                { LightDir->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }

	void SetSDFToWordInv0(CXMMATRIX M)                  { SDFToWordInv0->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetSDFBounds0(const XMFLOAT3& v)               { SDFBounds0->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetSDFRes0(const float f)                      { SDFRes0->SetFloat(f); }

	void SetGBuffer0(ID3D11ShaderResourceView* tex)     { GBuffer0->SetResource(tex); }
	void SetSDF0(ID3D11ShaderResourceView* tex)         { SDF0->SetResource(tex); }
	void SetDepthMap(ID3D11ShaderResourceView* tex)     { DepthMap->SetResource(tex); }
	void SetFrontDepthMap(ID3D11ShaderResourceView* tex){ FrontDepthMap->SetResource(tex); }
	void SetBackDepthMap(ID3D11ShaderResourceView* tex) { BackDepthMap->SetResource(tex); }

	ID3DX11EffectTechnique* SDFShadowTech;

	ID3DX11EffectMatrixVariable* ViewInv;
	ID3DX11EffectVectorVariable* LightDir;
	ID3DX11EffectVectorVariable* EyePosW;

	ID3DX11EffectMatrixVariable* SDFToWordInv0;
	ID3DX11EffectVectorVariable* SDFBounds0;
	ID3DX11EffectScalarVariable* SDFRes0;

	ID3DX11EffectShaderResourceVariable* DepthMap;
	ID3DX11EffectShaderResourceVariable* GBuffer0;
	ID3DX11EffectShaderResourceVariable* SDF0;
	ID3DX11EffectShaderResourceVariable* FrontDepthMap;
	ID3DX11EffectShaderResourceVariable* BackDepthMap;
};

#pragma endregion

#pragma  region SSREffect

class SSREffect	:public Effect
{
public:
	SSREffect(ID3D11Device* device, const std::wstring& filename);
	~SSREffect();

	void SetViewRot(CXMMATRIX M)						{ ViewRot->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetViewInv(CXMMATRIX M)						{ ViewInv->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetViewToTex(CXMMATRIX M)						{ ViewToTex->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetFarClipDist(float f)						{ FarClipDist->SetFloat(f); }

	void SetGBuffer0(ID3D11ShaderResourceView* tex)     { GBuffer0->SetResource(tex); }
	void SetDepthMap(ID3D11ShaderResourceView* tex)     { DepthMap->SetResource(tex); }
	void SetFrameBuffer(ID3D11ShaderResourceView* tex)  { FrameBuffer->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)		{ CubeMap->SetResource(tex); }
	void SetStencilMap(ID3D11ShaderResourceView* tex)	{ StencilMap->SetResource(tex); }

	ID3DX11EffectTechnique* SSRTech;

	ID3DX11EffectMatrixVariable* ViewRot;
	ID3DX11EffectMatrixVariable* ViewInv;
	ID3DX11EffectMatrixVariable* ViewToTex;
	ID3DX11EffectScalarVariable* FarClipDist;

	ID3DX11EffectShaderResourceVariable* DepthMap;
	ID3DX11EffectShaderResourceVariable* GBuffer0;
	ID3DX11EffectShaderResourceVariable* FrameBuffer;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* StencilMap;
};

#pragma endregion

#pragma region DebugTexEffect
class DebugTexEffect : public Effect
{
public:
	DebugTexEffect(ID3D11Device* device, const std::wstring& filename);
	~DebugTexEffect();

	void SetWorldViewProj(CXMMATRIX M)              { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexture(ID3D11ShaderResourceView* tex)  { Texture->SetResource(tex); }
	void SetSDF(ID3D11ShaderResourceView* tex)		{ SDF->SetResource(tex); }
	void SetIntTexture(ID3D11ShaderResourceView* tex)  { IntTexture->SetResource(tex); }
	void setD(float f){ d->SetFloat(f); }

	ID3DX11EffectTechnique* ViewArgbTech;
	ID3DX11EffectTechnique* ViewRedTech;
	ID3DX11EffectTechnique* ViewGreenTech;
	ID3DX11EffectTechnique* ViewBlueTech;
	ID3DX11EffectTechnique* ViewAlphaTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectScalarVariable* d;
	ID3DX11EffectShaderResourceVariable* Texture;
	ID3DX11EffectShaderResourceVariable* IntTexture;
	ID3DX11EffectShaderResourceVariable* SDF;
};
#pragma endregion

#pragma region SDFVolumeCulling

class SDFVolumeCullingEffect : public Effect
{
public:
	SDFVolumeCullingEffect(ID3D11Device* device, const std::wstring& filename);
	~SDFVolumeCullingEffect();

	void SetViewProj(CXMMATRIX M)						{ ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetFarClipDist(const float f)                      { FarClipDist->SetFloat(f); }

	ID3DX11EffectTechnique* SDFVolumeDepthTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectScalarVariable* FarClipDist;
};

#pragma  endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
	static NormalMapEffect* NormalMapFX;
	static BuildShadowMapEffect* BuildShadowMapFX;
	static SsaoNormalDepthEffect* SsaoNormalDepthFX;
	static SsaoEffect* SsaoFX;
	static SsaoBlurEffect* SsaoBlurFX;
	static SkyEffect* SkyFX;
	static DebugTexEffect* DebugTexFX;
	static GBufferEffect* GBufferFX;
	static DeferredShadingEffect* DeferredShadingFX;
	static SDFShadowEffect* SDFShadowFX;
	static SSREffect* SSRFX;
	static SDFVolumeCullingEffect* SDFVolumeCullingFX;
};
#pragma endregion

#endif // EFFECTS_H