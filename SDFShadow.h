#ifndef _SDFSHADOW
#define _SDFSHADOW
#include "d3dUtil.h"

class SDFShadow
{
public:
	SDFShadow(ID3D11Device* device, UINT width, UINT height);
	~SDFShadow();

	ID3D11ShaderResourceView* DepthMapSRV();

	void SetRenderTarget(ID3D11DeviceContext* dc);

	UINT mWidth;
	UINT mHeight;

	ID3D11ShaderResourceView* mSDFShadowSRV;
	ID3D11RenderTargetView* mSDFShadowRTV;

	D3D11_VIEWPORT mViewport;

};

#endif