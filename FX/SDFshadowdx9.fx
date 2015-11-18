
#include"GBufferUtil.fx"
texture3D gSDF0;
texture2D gDepthMap;
texture2D gGBuffer0;
texture2D gPreBuffer;

cbuffer cbPerFrame
{
	float4x4 gViewProj;
	float4x4 gView;
	float4x4 gViewInv;
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

struct VertexIn
{
	float3 PosH		: POSITION;
};

struct VertexOut
{
	float4 PosH			: POSITION;
	float3 viewDirW		: TEXCOORD1;
	float2 uv			: TEXCOORD2;
	float z				: TEXCOORD3;
};

uniform float3 zero = float3(-1e-4f, -1e-4f, -1e-4f);

bool inBounds(float3 start, float3 bounds)
{
	return all(start >= zero) && all(start <= bounds);
}

#define CULLING

bool IntersectBoundsCull(float3 start, float3 dir, float3 bounds,
	out float3 i1, out float length)
{
	i1 = float3(0, 0, 0);
	length = 0;
	float3 tMin = -start / dir;
	float3 tMax = (bounds - start) / dir;
	bounds += float3(1e-4f, 1e-4f, 1e-4f);
	float mint = 0, maxt = 0;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		if (inBounds(start + tMin[i] * dir, bounds))	{
			mint = min(tMin[i], mint);
			maxt = max(tMin[i], maxt);
		}
		if (inBounds(start + tMax[i] * dir, bounds))	{
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
	vout.viewDirW = mul(float4(vin.PosH,1.0f), gView).xyz;
	vout.viewDirW = mul(vout.viewDirW, (float3x3)gViewInv);//transform view space point to World vector
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

#define TOTALSTEP 32
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
	float length, len = 0;
	float4 shadow = float4(1.0f, 0, 0, 0);
	float boundMax = max(gSDFBounds0.x, max(gSDFBounds0.y, gSDFBounds0.z)) * 0.5f;
	float minstep;

	IntersectBoundsCull(posSDF, dir, gSDFBounds0, i1, length);
	{
		//return float4(0.0f,0,0,0);
		float3 start = i1;
		float3 scale = float3(1.0f,1.0f,1.0f) / gSDFBounds0;
		float step = gSDF0.Sample(samLinear, start * scale).r * boundMax;
		float smallstep = length / TOTALSTEP;
		//if (step < 0.00f)
		//{
		//	return shadow;
		//}
		//if (step < smallstep) step = smallstep;
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
	}
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

technique SDFShadowTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_3_0, VS()));
		SetPixelShader(CompileShader(ps_3_0, PS()));
	}
};



