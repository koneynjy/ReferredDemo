
#include"GBufferUtil.fx"
texture3D gSDF0;
texture2D gDepthMap;
texture2D gGBuffer0;

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
	return start.x >= -0.0001 & start.y >= -0.0001 & start.z >= -0.0001 &
		start.x <= bounds.x + 0.0001  &  start.y <= bounds.y + 0.0001 & start.z <= bounds.z + 0.0001;
}

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

	if (mint >= 0){
		float t1 = 0, t2 = t[maxt];
		i1 = start;
		float3 i2 = pInsec[maxt];
		if (t[maxt] - t[mint] > 0.0001f){
			i1 = pInsec[mint];
			t1 = t[mint];
		}
		length = t2 - t1;
		steps = (int)(length / 0.01f);
		return true;
	}

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

float4 PS(VertexOut pin) : SV_Target
{
	float3 v = pin.viewDirW * gDepthMap.Sample(samLinear, pin.uv).r;
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
	
	
	if (IntersectBounds(posSDF, dir, gSDFBounds0, i1, steps, length))
	{
		//return float4(1.0f,0,0,0);
		float3 start = i1;
		float step = gSDF0.Sample(samLinear, start / gSDFBounds0).r;
		if (step <= 0) return shadow;
		start += step * dir;
		[unroll(50)]
		for (int i = 0; i <= steps; ++i)
		{
			float3 uvw = start / gSDFBounds0;
			step = gSDF0.Sample(samLinear, uvw).r;
			if (step <= 0){
				shadow.r = 0.0f;
				break;
			}
			if (len >= length) break;
			start += step * dir;
		}
	}
	//shadow = (abs(gSDF0.Sample(samLinear, float3(0.5f, 0.5f, 0.5f)).r) / 3.0f).rrrr;
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



