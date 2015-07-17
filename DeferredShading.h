#ifndef _DEFERREDSHADING
#define _DEFERREDSHADING
#include "d3dUtil.h"
#include "Camera.h"

class DeferredShading
{
public:
	DeferredShading(ID3D11Device* device, UINT width, UINT height);
	~DeferredShading();

	void SetMRT(ID3D11DeviceContext* dc);
	UINT mWidth;
	UINT mHeight;
	D3D11_VIEWPORT mViewport;

	ID3D11ShaderResourceView* mGBufferSRV0;
	ID3D11ShaderResourceView* mGBufferSRV1;
	ID3D11ShaderResourceView* mDepthMapSRV;

	ID3D11RenderTargetView* mGBufferRTV0;
	ID3D11RenderTargetView* mGBufferRTV1;
	ID3D11DepthStencilView* mDepthMapDSV;
	
};

#endif // !_DEFERREDSHADING
