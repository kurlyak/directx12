Texture2D    gDiffuseMap : register(t0);

SamplerState gsamPointWrap  : register(s0);
SamplerState gsamPointClamp  : register(s1);
SamplerState gsamLinearWrap  : register(s2);
SamplerState gsamLinearClamp  : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp  : register(s5);


cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
	float4x4 gWorldView;
};

struct VertexIn
{
	float3 PosL  : POSITION;
    //float3 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float2 Tex : TEXCOORD;
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	float2 Position = sign(vin.PosL.xy);
    vout.PosH = float4(Position.xy,0, 1);

    vout.Tex.x=0.5* (1+vout.PosH.x);
    vout.Tex.y=0.5* (1-vout.PosH.y);

    
    return vout;
}


float4 PS(VertexOut pin) : SV_Target
{
	float4 ResColor =  gDiffuseMap.Sample(gsamLinearWrap, pin.Tex);

	return ResColor;
}


