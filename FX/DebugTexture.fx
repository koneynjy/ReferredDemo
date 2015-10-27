//=============================================================================
// DebugTexture.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Effect used to view a texture for debugging.
//=============================================================================
 
float4x4  gWorldViewProj;
Texture2D gTexture;
Texture2D<uint4> gIntTexture;
Texture3D gSDF;

cbuffer cbPerFrame
{
	float d;
};

cbuffer cbPerSDF
{
	float maxvalue;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex  : TEXCOORD;
};
 
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.PosH = float4(vin.PosL.xy,1.0f, 1.0f);
	vout.Tex  = vin.Tex;
	
	return vout;
}
[earlydepthstencil]
float4 PS(VertexOut pin) : SV_Target
{ 
	/*uint s = gIntTexture[pin.Tex * int2(800,600)].y;
	float sf = s / 255.0f ;
	return sf.rrrr;*/
	//return gTexture.Sample(samLinear, pin.Tex).rrrr;
	float v = gSDF.Sample(samLinear, float3(pin.Tex.x, 1.0f - pin.Tex.y, d)).r;
	if (v > 0)
	{
		v *= 0.5f;
	}
	else
	{
		v *= -0.5f;
		v = 1.0f - v;
	}

	return v.rrrr;
}

float4 PS(VertexOut pin, uniform int index) : SV_Target
{
	float4 c = gTexture.Sample(samLinear, pin.Tex);
	
	// draw as grayscale
	float g = c[index];
	return float4(g.rrr, 1);
}
 
technique11 ViewArgbTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

technique11 ViewRedTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(0) ) );
    }
}

technique11 ViewGreenTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(1) ) );
    }
}

technique11 ViewBlueTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(2) ) );
    }
}

technique11 ViewAlphaTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(3) ) );
    }
}