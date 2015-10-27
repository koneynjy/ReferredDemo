
#include"GBufferUtil.fx"
texture3D gSDF0;
texture2D gDepthMap;
texture2D gGBuffer0;
texture2D gFrontDepthMap;
texture2D gBackDepthMap;

cbuffer cbPerFrame
{
	float4x4 gViewInv;
	float3 gLightDir;
	float3 gEyePosW;
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
	float4 PosH			: SV_POSITION;
	float3 viewDirW		: POSITION;
	float2 uv			: TEXCORD0;
};

bool inBounds(float3 start, float3 bounds)
{
	return start.x >= -0.00001 & start.y >= -0.00001 & start.z >= -0.00001 &
		start.x <= bounds.x + 0.00001  &  start.y <= bounds.y + 0.00001 & start.z <= bounds.z + 0.00001;
}

#define CULLING

bool IntersectBounds(float3 start, float3 dir, float3 bounds,
	out float3 i1, out int steps, out float length)
{
	i1 = float3(0, 0, 0);
	steps = 0;
	length = 0;
	float3 tMin = -start / dir;
	float3 tMax = (bounds - start) / dir;
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
		steps = (int)(length / 0.2f);
		return true;
#ifndef CULLING
	}
#endif

	return false;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.viewDirW = mul(vin.PosH, (float3x3)gViewInv);//transform view space point to World vector
	vin.PosH.xy /= abs(vin.PosH.xy);//to -1 ~ 1
	vout.PosH = float4(vin.PosH.xy, 1.0f, 1.0f);
	vout.uv = vin.PosH.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);//screen cord uv

	return vout;
}

float CalcShadowFactor(float f)
{
	return f - 0.1f;
}

#define TOTALSTEP 128

float4 PS(VertexOut pin) : SV_Target
{

#ifdef CULLING
	float dback = gBackDepthMap.Sample(samLinear, pin.uv).r;
	float dfront = gFrontDepthMap.Sample(samLinear, pin.uv).r;
	bool bf = dfront >= 1.0f;
	clip(dback >= 1.0f && bf ? -1 : 1);
	dfront = bf ? 0.0f : dfront;
#endif
	float depth = gDepthMap.Sample(samLinear, pin.uv).r;
#ifdef CULLING
	clip(depth <= dback && depth >= dfront ? 1 : -1);
#endif
	float3 v = pin.viewDirW * depth;
	//return v.zzzz / 50;
	float3 posW = v + gEyePosW;//ReBuild World Position
	float3 posSDF = mul(float4(posW, 1.0), gSDFToWordInv0).xyz;
	float4 gb0 = gGBuffer0.Sample(samLinear, pin.uv);
	float3 normalW = GetNormal(gb0);
	float3 dir = -gLightDir;
	clip(dot(normalW, dir));
	float3 i1 = float3(0,0,0), i2;
	int steps;
	float length, len = 0;
	float4 shadow = float4(1.0f, 0, 0, 0);
	float boundMax = max(gSDFBounds0.x, max(gSDFBounds0.y, gSDFBounds0.z)) * 0.5f;
	float minstep;
#ifdef CULLING
	IntersectBounds(posSDF, dir, gSDFBounds0, i1, steps, length);
#else
	if (IntersectBounds(posSDF, dir, gSDFBounds0, i1, steps, length))
	{
#endif
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
	
	if (minstep < 1.0f)
		return shadow * minstep;
	return shadow;
}

technique11 SDFShadowTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};



