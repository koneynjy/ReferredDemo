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
	float z:		TEXCOORD0;
};

VertexOut VS(float3 posH: POSITION)
{
	VertexOut vout;
	vout.pos =  mul(float4(posH, 1.0f), gViewProj);
	vout.z = vout.pos.w;
	return vout;
}

float PS(VertexOut pin) : SV_Target
{
	return pin.z / gFarClipDist;
}

technique11 SDFVolumeDepthTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};