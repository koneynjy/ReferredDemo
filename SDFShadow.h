#ifndef _SDFSHADOW
#define _SDFSHADOW
#include "d3dUtil.h"
#include "Camera.h"
#include "GeometryGenerator.h"

class SDFShadow
{
public:
	SDFShadow(ID3D11Device* device, UINT width, UINT height);
	~SDFShadow();

	void SetRenderTarget(ID3D11DeviceContext* dc);
	void ClearShadow(ID3D11DeviceContext* dc);
	void SetRenderTarget(ID3D11DeviceContext* dc, bool cull);

	void GetCullingVolume(ID3D11Device* device, XMFLOAT3&center, XMFLOAT3& extend, 
		XMFLOAT3& lightDir, Camera& camera, XMFLOAT4X4& worldMat);
	UINT mWidth;
	UINT mHeight;

	ID3D11ShaderResourceView* mSDFShadowSRV;
	ID3D11RenderTargetView* mSDFShadowRTV;

	ID3D11ShaderResourceView* mSDFLiVolDepthBackSRV; //cull front
	ID3D11RenderTargetView* mSDFLiVolDepthBackRTV;

	ID3D11ShaderResourceView* mSDFLiVolDepthFrontSRV;//cull back
	ID3D11RenderTargetView* mSDFLiVolDepthFrontRTV;

	D3D11_VIEWPORT mViewport;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	UINT mIndexCount;

};

#endif