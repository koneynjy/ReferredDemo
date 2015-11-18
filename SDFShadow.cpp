#include "SDFShadow.h"

SDFShadow::SDFShadow(ID3D11Device* device, UINT width, UINT height)
: mWidth(width), mHeight(height), mSDFShadowSRV(0), mSDFShadowRTV(0)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(width);
	mViewport.Height = static_cast<float>(height);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	// Use typeless format because the DSV is going to interpret
	// the bits as DXGI_FORMAT_D24_UNORM_S8_UINT, whereas the SRV is going to interpret
	// the bits as DXGI_FORMAT_R24_UNORM_X8_TYPELESS.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* sdfpassmap = 0;
	HR(device->CreateTexture2D(&texDesc, 0, &sdfpassmap));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	HR(device->CreateRenderTargetView(sdfpassmap, &rtvDesc, &mSDFShadowRTV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	HR(device->CreateShaderResourceView(sdfpassmap, &srvDesc, &mSDFShadowSRV));
	ReleaseCOM(sdfpassmap);
	HR(device->CreateTexture2D(&texDesc, 0, &sdfpassmap));
	HR(device->CreateRenderTargetView(sdfpassmap, &rtvDesc, &mPreSDFShadowRTV));
	HR(device->CreateShaderResourceView(sdfpassmap, &srvDesc, &mPreSDFShadowSRV));
	ReleaseCOM(sdfpassmap);
	// View saves a reference to the texture so we can release our reference.
	texDesc.Format = DXGI_FORMAT_R16_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

// 	ID3D11Texture2D* bkmap = 0, *ftmap = 0;
// 	HR(device->CreateTexture2D(&texDesc, 0, &bkmap));
// 	HR(device->CreateTexture2D(&texDesc, 0, &ftmap));
// 
// 	rtvDesc.Format = DXGI_FORMAT_R16_UNORM;
// 	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
// 	rtvDesc.Texture2D.MipSlice = 0;
// 	HR(device->CreateRenderTargetView(bkmap, &rtvDesc, &mSDFLiVolDepthBackRTV));
// 	HR(device->CreateRenderTargetView(ftmap, &rtvDesc, &mSDFLiVolDepthFrontRTV));
// 
// 	srvDesc.Format = DXGI_FORMAT_R16_UNORM;
// 	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
// 	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
// 	srvDesc.Texture2D.MostDetailedMip = 0;
// 	HR(device->CreateShaderResourceView(bkmap, &srvDesc, &mSDFLiVolDepthBackSRV));
// 	HR(device->CreateShaderResourceView(ftmap, &srvDesc, &mSDFLiVolDepthFrontSRV));
// 
// 	
// 	ReleaseCOM(bkmap);
// 	ReleaseCOM(ftmap);
}

void SDFShadow::SetRenderTarget(ID3D11DeviceContext* dc, ID3D11DepthStencilView* dsv)
{
// 	std::swap(mSDFShadowRTV, mPreSDFShadowRTV);
// 	std::swap(mSDFShadowSRV, mPreSDFShadowSRV);
	dc->RSSetViewports(1, &mViewport);
	ID3D11RenderTargetView* renderTargets[1] = { mSDFShadowRTV };
	dc->OMSetRenderTargets(1, renderTargets, dsv);
// 	static FLOAT c[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
// 	dc->ClearRenderTargetView(mSDFShadowRTV, c);
}

void SDFShadow::SetRenderTarget(ID3D11DeviceContext* dc, bool cull)//false cull back; true cull front
{
// 	dc->RSSetViewports(1, &mViewport);
// 	ID3D11RenderTargetView* renderTargets[2] = { mSDFLiVolDepthFrontRTV, mSDFLiVolDepthBackRTV };
// 	dc->OMSetRenderTargets(1, renderTargets + cull, 0);
// 	static FLOAT c[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
// 	dc->ClearRenderTargetView(renderTargets[cull], c);
}

void SDFShadow::ClearShadow(ID3D11DeviceContext* dc){
	static FLOAT c[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	dc->ClearRenderTargetView(mSDFShadowRTV, c);
}

static XMFLOAT3 vt[8] = 
{
	XMFLOAT3( 1, -1, -1),
	XMFLOAT3( 1,  1, -1),
	XMFLOAT3(-1,  1, -1),
	XMFLOAT3(-1, -1, -1),
	XMFLOAT3( 1, -1,  1),
	XMFLOAT3( 1,  1,  1),
	XMFLOAT3(-1,  1,  1),
	XMFLOAT3(-1, -1,  1),
};

SDFShadow::~SDFShadow()
{
	ReleaseCOM(mSDFShadowRTV);
	ReleaseCOM(mSDFShadowSRV);
// 	ReleaseCOM(mSDFLiVolDepthBackRTV);
// 	ReleaseCOM(mSDFLiVolDepthBackSRV);
// 	ReleaseCOM(mSDFLiVolDepthFrontRTV);
// 	ReleaseCOM(mSDFLiVolDepthFrontSRV);
}
