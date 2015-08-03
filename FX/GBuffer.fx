#include "LightHelper.fx"
#include "GBufferUtil.fx"

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
};

cbuffer cbPerFrame
{
	float3 gEyePosW;
	float gFarClipDist;
};

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[96];
};

Texture2D gDiffuseMap;
Texture2D gNormalMap;
TextureCube gCubeMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL		: POSITION;
	float3 NormalL	: NORMAL;
	float2 Tex		: TEXCOORD;
	float4 TangentL	: TANGENT;
};

struct VertexInBase
{
	float3 PosL		: POSITION;
	float3 NormalL	: NORMAL;
	float2 Tex		: TEXCOORD;
};

struct SkinnedVertexIn
{
	float3 PosL       : POSITION;
	float3 NormalL    : NORMAL;
	float2 Tex        : TEXCOORD;
	float4 TangentL   : TANGENT;
	float3 Weights    : WEIGHTS;
	uint4 BoneIndices : BONEINDICES;
};

struct VertexOut
{
	float4 PosH			: SV_POSITION;
	float3 NormalW		: NORMAL;
	float4 TangentW		: TANGENT;
	float2 Tex			: TEXCOORD0;
	float z 			: TEXCOORD1;
};

struct VertexOutBase
{
	float4 PosH			: SV_POSITION;
	float3 NormalW		: NORMAL;
	float3 PosW			: Position;
	float2 Tex			: TEXCOORD0;
	float	z			: TEXCOORD1;
};


///////////////////////// vs no nmap////////////////
VertexOutBase VSBase(VertexInBase vin)
{
	VertexOutBase vout;

	//vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.PosW = mul(vin.PosL, (float3x3)gWorld);
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.z = vout.PosH.w;
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.TangentW = mul(vin.TangentL, gWorld);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.z = vout.PosH.w;
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}

VertexOut SkinnedVS(SkinnedVertexIn vin)
{
	VertexOut vout;

	// Init array or else we get strange warnings about SV_POSITION.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = vin.Weights.x;
	weights[1] = vin.Weights.y;
	weights[2] = vin.Weights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	float3 tangentL = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		// Assume no nonuniform scaling when transforming normals, so 
		// that we do not have to use the inverse-transpose.

		posL		+= weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
		normalL		+= weights[i] * mul(vin.NormalL, (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
		tangentL	+= weights[i] * mul(vin.TangentL.xyz, (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
	}

	// Transform to world space space.
	//vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(normalL, (float3x3)gWorldInvTranspose);
	vout.TangentW = float4(mul(tangentL, (float3x3)gWorld), vin.TangentL.w);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);
	vout.z = vout.PosH.w;
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}

void PS(VertexOut pin
	,out float4 c0 : SV_Target0
	,out float4 c1 : SV_Target1
	,out float z : SV_Depth
	)
{
	pin.NormalW = normalize(pin.NormalW);
	float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
	float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
	c0 = StoreGBufferRT0(bumpedNormalW, gMaterial.Specular.w);
	c1 = StoreGBufferRT1(gDiffuseMap.Sample(samLinear, pin.Tex).xyz * gMaterial.Diffuse.xyz, gMaterial.Specular.xyz);
	z = pin.z / gFarClipDist;
}

///////////////////base no nmap///////////
void PSBase(VertexOutBase pin
	,uniform bool gRefEnable
	,out float4 c0 : SV_Target0
	,out float4 c1 : SV_Target1
	,out float z : SV_Depth
	)
{
	pin.NormalW = normalize(pin.NormalW);
	c0 = StoreGBufferRT0(pin.NormalW, gMaterial.Specular.w);
	float4 diff = gMaterial.Diffuse;;
	if (gRefEnable){
		float3 incident = normalize(pin.PosW - gEyePosW);
		float3 reflectionVector = reflect(incident, pin.NormalW);
		float4 reflectionColor = gCubeMap.Sample(samLinear, reflectionVector);

		diff += gMaterial.Reflect*reflectionColor;
	}
	c1 = StoreGBufferRT1(diff.xyz, gMaterial.Specular.xyz);
	z = pin.z / gFarClipDist;
}


technique11 GBufferTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

technique11 GBufferTechSkinned
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

technique11 GBufferTechBase
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSBase()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSBase(false)));
	}
}

technique11 GBufferTechBaseReflect
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSBase()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSBase(true)));
	}
}