#include "GBufferUtil.fx"

cbuffer cbPerFrame
{
	float4x4 gViewToTex;
	float4x4 gViewRot;
	float4x4 gViewInv;
	float gFarClipDist;
	//float3 gEyePosW;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDepthMap;
Texture2D<uint4> gStencilMap;
Texture2D gFrameBuffer;
Texture2D gGBuffer0;
TextureCube gCubeMap;


SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samLinearClamp
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VertexIn
{
	float3 PosH		: POSITION;
};

struct VertexOut
{
	float4 PosH			: SV_POSITION;
	float3 viewDirV		: POSITION;
	float2 uv			: TEXCORD0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.viewDirV = vin.PosH;
	vin.PosH.xy /= abs(vin.PosH.xy);//to -1 ~ 1
	vout.PosH = float4(vin.PosH.xy, 1.0f, 1.0f);
	vout.uv = vin.PosH.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);//screen cord uv

	return vout;
}

#define BSSTEPS 5
float4 binarySearch(float3 posL, float3 posR)
{
	float3 posM = (posL + posR) / 2.0f;;
	float4 posH = mul(float4(posM, 1.0f), gViewToTex);
	posH /= posH.w;
	for (int i = 0; i < BSSTEPS; i++)
	{
		float dz = posM.z - gDepthMap.Sample(samLinearClamp, posH.xy).r * gFarClipDist;
		if (dz < 0)
		{
			posL = posM;
		}
		else
		{
			posR = posM;
		}
		posM = (posL + posR) / 2.0f;
		posH = mul(float4(posM, 1.0f), gViewToTex);
		posH /= posH.w;
	}

	return 	gFrameBuffer.Sample(samLinearClamp, posH.xy);
	
}

uniform int gId = 1;
#define STEPS 10
#define MINSTEP 1.0f
float4 PS(VertexOut pin) : SV_Target
{
	uint2 res;
	gStencilMap.GetDimensions(res.x, res.y);
	int s = gStencilMap[res * pin.uv].g;
	clip(s == gId ? 1 : -1);
	//clip senceil 
	
	float depth = gDepthMap.Sample(samLinear, pin.uv).r;
	float3 posV = pin.viewDirV * depth;
	float4 gb0 = gGBuffer0.Sample(samLinear, pin.uv);
		//return float4(1.0f,0,0,0);
	float3 normalV = mul(GetNormal(gb0), (float3x3)gViewRot);
	float3 rv = reflect(normalize(posV), normalV);	//reflect vector
	float3 rvW = mul(rv, (float3x3)gViewInv);
	float4 retValue = gCubeMap.Sample(samLinear, rvW);
	float3 len;
	if (rv.z > 0.0f)
	{
		//return rv.zzzz;
		//return float4(1.0f, 0, 0, 0);
		len = rv * ((50 - posV.z) / rv.z);  //get the total length
		float3 step = len / STEPS;
		float lz = 0;
		[unroll]
		for (int i = 0; i < STEPS; i++){
			//return float4(1.0f, 0, 0, 0);
			posV += step;
			float4 posH = mul(float4(posV, 1.0f), gViewToTex);
			posH /= posH.w;
			if (posH.x > 1.0f || posH.x < 0 || posH.y > 1.0f || posH.y < 0)
			{
				return retValue;
			}
			float dz = posV.z - gDepthMap.Sample(samLinearClamp, posH.xy).r * gFarClipDist;
			[flatten]
			if (dz > 0){
				//return float4(1.0f, 0, 0, 0);
				return binarySearch(posV - step, posV);
				//break;
			}
			lz = dz;
		}
	}

	return retValue;// 
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