cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
	float4x4 gWorldView;
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float3 Normal : NORMAL;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float3 tNormal : NORMAL;
	float3 PosW : POSITION;
};

static float3 LightPos = { 0.0f, 0.0f, 0.0f };
static float3 DiffuseLightColor = { 1.0f, 1.0f, 0.5f };
static float3 AmbientLightColor = { 1.0f, 1.0f, 0.5f };
static float3 SpecularLightColor = { 0.15f, 0.15f, 0.15f };
static float BrightnessDiffuse = 0.65f;
static float BrightnessAmbient = 0.5f;
static float ShininessSpecular = 3;

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	float4 PosW = mul(float4(vin.PosL, 1.0f), gWorldView);
	vout.PosW = PosW.xyz;
	
    vout.tNormal = mul(vin.Normal, (float3x3)gWorldView);
    
    return vout;
}

float3 Get_ADS_Color(float3 tNormal, float3 PosW)
{
	float3 Normal = normalize(tNormal);
	float3 ToLightPos = normalize(LightPos - PosW);
	float3 Pos = normalize(-PosW);
	float3 Reflect = reflect(-ToLightPos, Normal);
	
	float Dot = max(dot(ToLightPos, Normal), 0.0f);

	//calculate diffuse
	float3 DiffuseColor = DiffuseLightColor * Dot * BrightnessDiffuse;
	
	//calculate specular
	float3 SpecularColor = { 0.0f, 0.0f, 0.0f };

	if(Dot > 0)
		SpecularColor = SpecularLightColor * pow(max(dot(Reflect, Pos), 0.0), ShininessSpecular);

	//calculate ambient
	float3 AmbientColor = AmbientLightColor* BrightnessAmbient;

	return AmbientColor + DiffuseColor + SpecularColor;
}

float4 PS(VertexOut pin) : SV_Target
{

	float3 ResColor = Get_ADS_Color(pin.tNormal, pin.PosW);
	
	return float4(ResColor, 1.0f);

}


