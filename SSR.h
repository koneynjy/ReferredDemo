#ifndef _SSR_H
#define _SSR_H
#include "d3dUtil.h"
class SSR
{
public:
	SSR(ID3D11Device* device, UINT width, UINT height);
	~SSR();

	void SetRenderTarget(ID3D11DeviceContext* dc, ID3D11DepthStencilView* dsv);
	void ClearSSR(ID3D11DeviceContext* dc);
	UINT mWidth;
	UINT mHeight;

	ID3D11ShaderResourceView* mSSRSRV;
	ID3D11RenderTargetView* mSSRRTV;

	D3D11_VIEWPORT mViewport;

private:

};


#endif