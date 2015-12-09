
#include"GBufferUtil.fx"
texture3D gSDF0;
texture2D gDepthMap;
texture2D gGBuffer0;
texture2D gPreBuffer;

cbuffer cbPerFrame
{
	float4x4 gViewProj;
	float4x4 gView;	//deprecated
	float4x4 gViewInv; //deprecated
	float3 gLightDir;
	float3 gEyePosW;
	float  gFarClipDist;
};

cbuffer cbPerModel
{
	float4x4 gSDFToWordInv0;
	float3 gSDFBounds0;
	float gSDFRes0;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

RasterizerState	cullFront
{
	CullMode = FRONT;
};

RasterizerState	cullBack
{
	CullMode = Back;
};

DepthStencilState cfDSS
{
	DepthEnable = true;
	DepthWriteMask = ZERO;
	DepthFunc = GREATER_EQUAL;
	StencilEnable = true;
	StencilReadMask = 0xff;
	StencilWriteMask = 0xff;
	FrontFaceStencilFail = KEEP;
	FrontFaceStencilDepthFail = KEEP;
	FrontFaceStencilPass = REPLACE;
	FrontFaceStencilFunc = ALWAYS;
	BackFaceStencilFail = KEEP;
	BackFaceStencilDepthFail = KEEP;
	BackFaceStencilPass = REPLACE;
	BackFaceStencilFunc = ALWAYS;
};

DepthStencilState cbDSS
{
	DepthEnable = true;
	DepthWriteMask = ZERO;
	DepthFunc = LESS_EQUAL;
	StencilEnable = true;
	StencilReadMask = 0xff;
	StencilWriteMask = 0xff;
	FrontFaceStencilFail = KEEP;
	FrontFaceStencilDepthFail = ZERO;
	FrontFaceStencilPass = KEEP;
	FrontFaceStencilFunc = EQUAL;
	BackFaceStencilFail = KEEP;
	BackFaceStencilDepthFail = KEEP;
	BackFaceStencilPass = REPLACE;
	BackFaceStencilFunc = NEVER;
};

struct VertexIn
{
	float3 PosH		: POSITION;
};

struct VertexOut
{
	float4 PosH			: SV_POSITION;
	float3 viewDirW		: POSITION0;
	float2 uv			: POSITION1;
	float z				: TEXCOORD0;
};

void CullVS(float3 posH: POSITION
	, out float4 pos : SV_POSITION)
{
	pos = mul(float4(posH, 1.0f), gViewProj);
}

uniform float3 zero = float3(-1e-4f, -1e-4f, -1e-4f);

bool inBounds(float3 start, float3 bounds)
{
	return all(start >= zero) && all(start <= bounds);
}

#define CULLING

bool IntersectBounds(float3 start, float3 dir, float3 bounds,
	out float3 i1, out float length)
{
	i1 = float3(0, 0, 0);
	length = 0;
	float3 tMin = -start / dir;
	float3 tMax = (bounds - start) / dir;
	bounds += float3(1e-4f, 1e-4f, 1e-4f);
	float3 pInsec[6];
	float t[6];
	int cnt = 0, mint = -1, maxt = -1;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		float3 pTmp;
		if (tMin[i] >= 0){
			pTmp = start + tMin[i] * dir;
			if (inBounds(pTmp, bounds))
			{
				pInsec[cnt] = pTmp;
				t[cnt] = tMin[i];
				if (mint < 0 || t[cnt] < t[mint]) mint = cnt;
				if (maxt < 0 || t[cnt] > t[maxt]) maxt = cnt;
				++cnt;
			}
		}

		if (tMax[i] >= 0){
			pTmp = start + tMax[i] * dir;
			if (inBounds(pTmp, bounds))
			{
				pInsec[cnt] = pTmp;
				t[cnt] = tMax[i];
				if (mint < 0 || t[cnt] < t[mint]) mint = cnt;
				if (maxt < 0 || t[cnt] > t[maxt]) maxt = cnt;
				++cnt;
			}
		}
	}
#ifndef CULLING
	if (mint >= 0){
#endif
		float t1 = 0, t2 = t[maxt];
		i1 = start;
		float3 i2 = pInsec[maxt];
		if (t[maxt] - t[mint] > 0.0001f){
			i1 = pInsec[mint];
			t1 = t[mint];
		}
		length = t2 - t1;
		return true;
#ifndef CULLING
	}
#endif

	return false;
}


bool IntersectBoundsCull(float3 start, float3 dir, float3 bounds,
	out float3 i1, out float length)
{
	i1 = float3(0, 0, 0);
	length = 0;
	float3 tMin = -start / dir;
	float3 tMax = (bounds - start) / dir;
	bounds += float3(1e-4f, 1e-4f, 1e-4f);
	float mint = 3.40282e+038f, maxt = 0;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		if (inBounds(start + tMin[i] * dir, bounds))
		{
			mint = min(tMin[i], mint);
			maxt = max(tMin[i], maxt);
		}

		if (inBounds(start + tMax[i] * dir, bounds))
		{
			mint = min(tMax[i], mint);
			maxt = max(tMax[i], maxt);
		}
	}

	mint = max(0, mint);
	i1 = start + dir * mint;
	length = maxt - mint;
	return true;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.viewDirW = vin.PosH - gEyePosW;//mul(float4(vin.PosH,1.0f), gView).xyz;
	//vout.viewDirW = mul(vout.viewDirW, (float3x3)gViewInv);//transform view space point to World vector
	//vin.PosH.xy /= abs(vin.PosH.xy);//to -1 ~ 1
	vout.PosH = mul(float4(vin.PosH, 1.0f), gViewProj);
	vout.uv = vout.PosH.xy;
	vout.z = vout.PosH.w;
	//vout.cPos.xy = vout.cPos.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);//for computing screen cord uv
	return vout;
}

float CalcShadowFactor(float f)
{
	return f - 0.1f;
}

#define TOTALSTEP 16
[earlydepthstencil]
float4 PS(VertexOut pin) : SV_Target
{
	//return 0.5.rrrr;
	float2 uv = pin.uv / pin.z;
	uv = uv * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	float depth = gDepthMap.Sample(samLinear, uv).r;

	float3 v = pin.viewDirW * (depth * gFarClipDist / pin.z);
		//return v.zzzz / 50;
	float3 posW = v + gEyePosW;//ReBuild World Position
	float3 posSDF = mul(float4(posW, 1.0), gSDFToWordInv0).xyz;
	float4 gb0 = gGBuffer0.Sample(samLinear, uv);
	float3 normalW = GetNormal(gb0);
	float3 dir = -gLightDir;
	clip(dot(normalW, dir));
	float3 i1 = float3(0,0,0), i2;
	float length = 0, len = 0;
	float4 shadow = float4(1.0f, 0, 0, 0);
	float boundMax = gSDFRes0;//max(gSDFBounds0.x, max(gSDFBounds0.y, gSDFBounds0.z)) * 0.5f;
	float minstep;
#ifdef CULLING
	IntersectBoundsCull(posSDF, dir, gSDFBounds0, i1, length);
#else
	if (IntersectBounds(posSDF, dir, gSDFBounds0, i1, length))
	{
#endif
		//return float4(i1/ gSDFBounds0, 1.0f);
		float3 start = i1;
		float3 scale = float3(1.0f,1.0f,1.0f) / gSDFBounds0;
		float step = gSDF0.Sample(samLinear, start * scale).r * boundMax;
		float smallstep = length / TOTALSTEP;
		float ori = 0.01f * boundMax;
		start += ori * dir;
		len += ori;
		minstep = ori * TOTALSTEP / len;
		[unroll(TOTALSTEP)]
		for (int i = 0; i <= TOTALSTEP; ++i)
		{
			if (len >= length) break;
			//float3 uvw = start * scale;
			step = gSDF0.Sample(samLinear, start * scale).r;
			if (step <= 0){
				shadow.r = 0.0f;
				break;
			}
			step *= boundMax;
			minstep = min(minstep, step * TOTALSTEP / len);
			
			if (step < smallstep) step = smallstep;
			//step *= 1.25f;
			len += step;
			start += step * dir;
		}
#ifndef	CULLING
	}
#endif
	//else
	//{
	//	shadow.r = 0.5f;
	//	return shadow;
	//}
	//shadow = (abs(gSDF0.Sample(samLinear, float3(0.5f, 0.5f, 0.5f)).r) / 3.0f).rrrr;
	//if (minstep < 0.1f)	 {
	//	shadow.r = 0.0f;
	//	return shadow;
	//}
	
	if (minstep < 1.0f) shadow *= minstep;	
	return shadow;
}

technique11 SDFShadowTech
{
	pass P0
	{
		SetRasterizerState(cullFront);
		SetDepthStencilState(cfDSS, 0xff);
		SetVertexShader(CompileShader(vs_5_0, CullVS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);
	}
	pass P1
	{
		SetRasterizerState(cullBack);
		SetDepthStencilState(cbDSS, 0xff);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};



