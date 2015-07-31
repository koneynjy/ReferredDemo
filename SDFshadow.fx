
texture3D gSDF0;
texture2D gDepthMap;

bool inBounds(float3 start, float3 bounds)
{
	return start.x >= 0 & start.y >= 0 & start.z >= 0 &
		start.x <= bounds.x &  bounds.y <= bMax.y & bounds.z <= bMax.z;
}

bool IntersectBounds(float3 start, float3 dir, float3 bounds)
{	
	float3 tMin = -start / dir;
	float3 tMax = (bounds - start) / dir;
}

cbuffer cbPerFrame
{
	float4x4 gViewInv;
	float3 gLightDir;
};

cbuffer cbPerModel
{
	float4x4 gSDFToWordInv0;
	float3 gSDFBounds0;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
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
	vout.viewDirW = mul(vin.PosH, (float3x3)gViewInv);//transform view space point to World vector
	vin.PosH.xy /= abs(vin.PosH.xy);//to -1 ~ 1
	vout.PosH = float4(vin.PosH.xy, 1.0f, 1.0f);
	vout.uv = vin.PosH.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);//screen cord uv

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float3 v = pin.viewDirW * (gDepthMap.Sample(samLinear, pin.uv).r);
	float3 posW = v + gEyePosW;//ReBuild World Position
	float3 posSDF = mul(posW, gSDFToWordInv0);
	if (IntersectBounds(posSDF, -gLightDir, gSDFBounds0))
	{

	}
	return litColor;
}

technique11 DeferredShadingTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};



