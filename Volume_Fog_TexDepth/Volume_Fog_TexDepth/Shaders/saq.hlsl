Texture2D    gDiffuseMap1 : register(t0);
Texture2D    gDiffuseMap2 : register(t1);

SamplerState gsamPointWrap  : register(s0);
SamplerState gsamPointClamp  : register(s1);
SamplerState gsamLinearWrap  : register(s2);
SamplerState gsamLinearClamp  : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp  : register(s5);

static float FogFactor = 15.0f;

struct VertexIn
{
	float3 PosL  : POSITION;
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
    vout.PosH = float4(Position.xy,0.0f, 1.0f);

    vout.Tex.x=0.5f * (1.0f + vout.PosH.x);
    vout.Tex.y=0.5f * (1.0f - vout.PosH.y);

    
    return vout;
}


float4 PS(VertexOut pin) : SV_Target
{
	float front = gDiffuseMap1.Sample(gsamLinearWrap, pin.Tex).r;
	float back = gDiffuseMap2.Sample(gsamLinearWrap, pin.Tex).r;
	
	float k = (back - front) * FogFactor;

	float3 FogColor = { 0.5f,0.5f,0.5f };

	return float4(FogColor * k, 1.0f);
}


