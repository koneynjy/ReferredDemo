//***************************************************************************************
// RenderStates.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "RenderStates.h"
#include "Effects.h"

ID3D11RasterizerState*   RenderStates::WireframeRS       = 0;
ID3D11RasterizerState*   RenderStates::NoCullRS          = 0;
ID3D11RasterizerState*   RenderStates::CullBackRS		 = 0;
ID3D11RasterizerState*   RenderStates::CullFrontRS		 = 0;
	 
ID3D11DepthStencilState* RenderStates::EqualsDSS         = 0;
ID3D11DepthStencilState* RenderStates::NoDepth			 = 0;
ID3D11DepthStencilState* RenderStates::LessDSS			 = 0;
ID3D11DepthStencilState* RenderStates::SSRDSS  = 0;

ID3D11BlendState*        RenderStates::AlphaToCoverageBS = 0;
ID3D11BlendState*        RenderStates::TransparentBS     = 0;

void RenderStates::InitAll(ID3D11Device* device)
{
	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//cull back
	D3D11_RASTERIZER_DESC cullBackDesc;
	ZeroMemory(&cullBackDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullBackDesc.FillMode = D3D11_FILL_SOLID;
	cullBackDesc.CullMode = D3D11_CULL_BACK;
	cullBackDesc.FrontCounterClockwise = false;
	cullBackDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&cullBackDesc, &CullBackRS));
	//cull front
	D3D11_RASTERIZER_DESC cullFrontDesc;
	ZeroMemory(&cullFrontDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullFrontDesc.FillMode = D3D11_FILL_SOLID;
	cullFrontDesc.CullMode = D3D11_CULL_FRONT;
	cullFrontDesc.FrontCounterClockwise = false;
	cullFrontDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&cullFrontDesc, &CullFrontRS));

	//
	// EqualsDSS
	//
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	equalsDesc.DepthFunc = D3D11_COMPARISON_EQUAL;

	HR(device->CreateDepthStencilState(&equalsDesc, &EqualsDSS));

	D3D11_DEPTH_STENCIL_DESC lessDesc;
	ZeroMemory(&lessDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	lessDesc.DepthEnable = true;
	lessDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	lessDesc.DepthFunc = D3D11_COMPARISON_LESS;
	lessDesc.StencilEnable = true;
	lessDesc.StencilReadMask = 0xff;
	lessDesc.StencilWriteMask = 0xff;
	lessDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	lessDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	lessDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	lessDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	lessDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	lessDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	lessDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	lessDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	HR(device->CreateDepthStencilState(&lessDesc, &LessDSS));
	//NoDepth

	D3D11_DEPTH_STENCIL_DESC noDesc;
	ZeroMemory(&noDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	noDesc.DepthEnable = false;
	noDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	noDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;


	HR(device->CreateDepthStencilState(&noDesc, &NoDepth));

	D3D11_DEPTH_STENCIL_DESC nDeSDesc;
	ZeroMemory(&noDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	nDeSDesc.DepthEnable = false;
	nDeSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	nDeSDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	nDeSDesc.StencilEnable = true;
	nDeSDesc.StencilReadMask = SSR_EFFECT;
	nDeSDesc.StencilWriteMask = 0xff;
	nDeSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	nDeSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	nDeSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	nDeSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
	nDeSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	nDeSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	nDeSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	nDeSDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	HR(device->CreateDepthStencilState(&nDeSDesc, &SSRDSS));
	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = {0};
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	//
	// TransparentBS
	//

	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&transparentDesc, &TransparentBS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(EqualsDSS);
	ReleaseCOM(CullFrontRS);
	ReleaseCOM(CullBackRS);
	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
}