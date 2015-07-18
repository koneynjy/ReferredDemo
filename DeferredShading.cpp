#include "DeferredShading.h"
#include "GeometryGenerator.h"
#include "Vertex.h"
#include "Effects.h"

DeferredShading::DeferredShading(ID3D11Device* device, UINT width, UINT height)
:mWidth(width), mHeight(height), mGBufferRTV0(0), mGBufferRTV1(0), 
mGBufferSRV0(0), mGBufferSRV1(0), mDepthMapSRV(0), mDepthMapDSV(0)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(width);
	mViewport.Height = static_cast<float>(height);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* depthMap = 0;
	HR(device->CreateTexture2D(&texDesc, 0, &depthMap));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HR(device->CreateDepthStencilView(depthMap, &dsvDesc, &mDepthMapDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	HR(device->CreateShaderResourceView(depthMap, &srvDesc, &mDepthMapSRV));

	// View saves a reference to the texture so we can release our reference.
	ReleaseCOM(depthMap);

	//gbuffer
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D *rt0 = 0, *rt1 = 0;
	HR(device->CreateTexture2D(&texDesc, 0, &rt0));
	HR(device->CreateTexture2D(&texDesc, 0, &rt1));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	HR(device->CreateRenderTargetView(rt0, &rtvDesc, &mGBufferRTV0));
	HR(device->CreateRenderTargetView(rt1, &rtvDesc, &mGBufferRTV0));

	D3D11_SHADER_RESOURCE_VIEW_DESC gsrvDesc;
	gsrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	gsrvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	gsrvDesc.Texture2D.MostDetailedMip = 0;
	HR(device->CreateShaderResourceView(rt0, &gsrvDesc, &mGBufferSRV0));
	HR(device->CreateShaderResourceView(rt1, &gsrvDesc, &mGBufferSRV1));
	// View saves a reference to the texture so we can release our reference.
	ReleaseCOM(depthMap);
	ReleaseCOM(rt0);
	ReleaseCOM(rt1);
}

DeferredShading::~DeferredShading()
{
	ReleaseCOM(mGBufferRTV0);
	ReleaseCOM(mGBufferRTV1);
	ReleaseCOM(mDepthMapDSV);

	ReleaseCOM(mGBufferSRV0);
	ReleaseCOM(mGBufferSRV1);
	ReleaseCOM(mDepthMapSRV);
}

void DeferredShading::SetMRT(ID3D11DeviceContext* dc){
	dc->RSSetViewports(1, &mViewport);
	ID3D11RenderTargetView* renderTargets[2] = { mGBufferRTV0, mGBufferRTV1 };
	dc->OMSetRenderTargets(2, renderTargets, mDepthMapDSV);
	FLOAT c[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	dc->ClearRenderTargetView(mGBufferRTV0, c);
	dc->ClearRenderTargetView(mGBufferRTV1, c);
	dc->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DeferredShading::InitQuad(Camera &camera, ID3D11Device* device)
{
	GeometryGenerator::MeshData quad;
	GeometryGenerator geoGen;
	geoGen.CreateFarPlaneQuad(quad, camera);

	std::vector<XMFLOAT3> vertices(quad.Vertices.size());

	for (size_t i = 0; i < quad.Vertices.size(); ++i)
	{
		vertices[i] = quad.Vertices[i].Position;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];

	HR(device->CreateBuffer(&vbd, &vinitData, &mVB));


	mIndexCount = quad.Indices.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	std::vector<USHORT> indices16;
	indices16.assign(quad.Indices.begin(), quad.Indices.end());

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices16[0];

	HR(device->CreateBuffer(&ibd, &iinitData, &mIB));
}