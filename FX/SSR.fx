#include "GBufferUtil.fx"

cbuffer cbPerFrame
{
	float4x4 gViewToTex;
	float4x4 gViewRot;
	float gFarClipDist;
	//float3 gEyePosW;
	int gId;
};

cbuffer cbFix
{
	float dL = 0.5f;
};


// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDepthMap;
Texture2D gFrameBuffer;
Texture2D gGBuffer0;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
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

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.viewDirW = vin.PosH;//transform view space point to World vector
	vin.PosH.xy /= abs(vin.PosH.xy);//to -1 ~ 1
	vout.PosH = float4(vin.PosH.xy, 1.0f, 1.0f);
	vout.uv = vin.PosH.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);//screen cord uv

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float2 ds = gDepthMap.Sample(samLinear, pin.uv).rg;
	clip(ds.g == gId ? 1 : -1);
	float3 posV = pin.viewDirW * ds.r;
	float4 gb0 = gGBuffer0.Sample(samLinear, pin.uv);
	float3 normalW = mul(GetNormal(gb0), (float3x3)gViewRot);
	float3 rv = reflect(posV, normalW) * dL;
	float lz = posV.z - ds.r * gFarClipDist;
	float4 ret = gFrameBuffer.Sample(samLinear, pin.uv);
	[unroll]
	for (int i = 0; i < 100; i++){
		posV += rv;
		float4 posH = mul(float4(posV, 1.0f), gViewToTex);
		posH /= posH.w;
		float dz = posV.z - gDepthMap.Sample(samLinear, posH.xy).r * gFarClipDist;
		[flatten]
		if (dz > 0){
			posV = posV - rv * (dz / (dz - lz));
			posH = mul(float4(posV, 1.0f), gViewToTex);
			posH /= posH.w;
			return ret * gFrameBuffer.Sample(samLinear, posH.xy);
		}
		lz = dz;
	}

	return ret;
}

technique11 SSRTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};