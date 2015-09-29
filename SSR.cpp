#include "SSR.h"


SSR::SSR(ID3D11Device* device, UINT width, UINT height)
:mWidth(width), mHeight(height), mSSRSRV(0), mSSRRTV(0)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(width >> 1);
	mViewport.Height = static_cast<float>(height >> 1);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	// Use typeless format because the DSV is going to interpret
	// the bits as DXGI_FORMAT_D24_UNORM_S8_UINT, whereas the SRV is going to interpret
	// the bits as DXGI_FORMAT_R24_UNORM_X8_TYPELESS.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth >> 1;
	texDesc.Height = mHeight >> 1;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* ssrmap = 0;
	HR(device->CreateTexture2D(&texDesc, 0, &ssrmap));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	HR(device->CreateRenderTargetView(ssrmap, &rtvDesc, &mSSRRTV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	HR(device->CreateShaderResourceView(ssrmap, &srvDesc, &mSSRSRV));

	// View saves a reference to the texture so we can release our reference.
	ReleaseCOM(ssrmap);
}

void SSR::SetRenderTarget(ID3D11DeviceContext* dc, ID3D11DepthStencilView* dsv)
{
	dc->RSSetViewports(1, &mViewport);
	ID3D11RenderTargetView* renderTargets[1] = { mSSRRTV };
	dc->OMSetRenderTargets(1, renderTargets, dsv);
	ClearSSR(dc);
}

void SSR::ClearSSR(ID3D11DeviceContext* dc)
{
	FLOAT c[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	dc->ClearRenderTargetView(mSSRRTV, c);
}

SSR::~SSR()
{
	ReleaseCOM(mSSRRTV);
	ReleaseCOM(mSSRSRV);
}