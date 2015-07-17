#include "LightHelper.fx"
#include "GBufferUtil.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float4x4 gViewInv;
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gShadowTransform;
	float4 amb;
	float4 ref;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gShadowMap;
Texture2D gSsaoMap;
Texture2D gDepthMap;
Texture2D gGBuffer0;
Texture2D gGBuffer1;
TextureCube gCubeMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS;
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
	vout.viewDirW = mul(float4(vin.PosH, 1.0f), gViewInv).xyz;//transform view space point to World vector
	vin.PosH.xy /= abs(vin.PosH.xy);//to -1 ~ 1
	vout.PosH = float4(vin.PosH.xy, 0.999f, 1.0f);
	vout.uv = vin.PosH.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);//screen cord uv

	return vout;
}

float4 PS(VertexOut pin, uniform bool gReflectionEnabled) : SV_Target
{
	float3 posW = pin.viewDirW * gDepthMap.Sample(samLinear, pin.uv).r + gEyePosW;//ReBuild World Position
	float4 gb0 = gGBuffer0.Sample(samLinear, pin.uv);
	float4 gb1 = gGBuffer1.Sample(samLinear, pin.uv);
	float3 normalW = GetNormal(gb0);
	float gloss = GetShininess(gb0);
	float4 diff = float4(gb1.xyz, 0.0f);
	float spec = gb1.w;
	float3 v = normalize(gEyePosW - posW);
	float4 shadowPosH = mul(float4(posW, 1.0f), gShadowTransform);
	// Default to multiplicative identity.
	//
	// Lighting.
	//

	float4 litColor;
	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Only the first light casts a shadow.
	float3 shadow = float3(1.0f, 1.0f, 1.0f);
	shadow[0] = CalcShadowFactor(samShadow, gShadowMap, shadowPosH);

	float ambientAccess = gSsaoMap.SampleLevel(samLinear, pin.uv, 0.0f).r;

	// Sum the light contribution from each light source.  
	[unroll]
	for (int i = 0; i < 3; ++i)
	{
		float4 A, D, S;
		ComputeDirectionalLight(amb, diff, spec, gloss, gDirLights[i], normalW, v,
			A, D, S);

		ambient += ambientAccess*A;
		diffuse += shadow[i] * D;
		specular += shadow[i] * S;
	}

	litColor = ambient + diffuse + spec;

	if (gReflectionEnabled)
	{
		float3 incident = -v;
		float3 reflectionVector = reflect(incident, normalW);
		float4 reflectionColor = gCubeMap.Sample(samLinear, reflectionVector);

		litColor += ref*reflectionColor;
	}

	return litColor;
}