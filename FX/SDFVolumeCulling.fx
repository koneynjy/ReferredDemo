cbuffer cbConstant
{
	float gFarClipDist;
};

cbuffer cbPerFrame
{
	float4x4 gViewProj;
};

struct VertexOut
{
	float4 pos:		SV_POSITION;
};

VertexOut VS(float3 posH: POSITION)
{
	VertexOut vout;
	vout.pos =  mul(float4(posH, 1.0f), gViewProj);
	return vout;
}

void PS(VertexOut pin){}

technique11 SDFVolumeDepthTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);
	}
};