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
	float3 gCamPos;
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float2 Tex : TEXCOORD;
    float fog_val : TEXCOORD1;
};

//static float3 cameraPos = float3(25.0f, 5.0f, -5000.0f);

float3 ComponentProd(float3 v1, float3 v2)
{
	return float3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}


float Check_Sphere(float3 vertexPos, float3 cameraPos)
{
	float fSphereRadius = 4000.0f;

	float3 vSphereCenter = float3(0,0,0);

	float3 m_Scale = float3(1.0f / fSphereRadius, 1.0f / fSphereRadius, 1.0f / fSphereRadius);

	float3 adjCameraPos = ComponentProd((cameraPos - vSphereCenter), m_Scale);
	float3 adjVertexPos = ComponentProd((vertexPos - vSphereCenter), m_Scale);

	float3 adjDistance = adjVertexPos - adjCameraPos;

	float OD = dot(adjDistance, adjCameraPos);
	float D2 = dot(adjDistance, adjDistance);
	float O2 = dot(adjCameraPos, adjCameraPos);


	float radix = OD*OD - D2*(O2 - 1);
	
	if (radix <= 0)
	{
		return 0.0f;
	}

	float sradix = float(sqrt(radix));

	//if (sradix <= 0)
	//if (radix <= 0)
	//	return 0.0f;

	float t1 = (-OD - sradix) / D2;
	float t2 = (-OD + sradix) / D2;

	float3 v1; 
	float3 v2; 


	if(t1 >= 0.0f && t1 < 1.0f && t2 > 0.0f && t2 <= 1.0f)
	{
		v1 = cameraPos + t1*(vertexPos - cameraPos);
		v2 = cameraPos + t2*(vertexPos - cameraPos);
	
		float3 vDist = v2 - v1;

		float val = length(vDist);

		return val;
	}


	if(t1 >= 0.0f && t1 < 1.0f && t2 > 1.0f)
	{

		t2 = 1.0f;

		v1 = cameraPos + t1*(vertexPos - cameraPos);
		v2 = cameraPos + t2*(vertexPos - cameraPos);
	
		float3 vDist = v2 - v1;

		float val = length(vDist);

		return val;
	}

	if(t1 < 0.0f && t2 > 0.0f && t2 <= 1.0f)
	{
		t1 = 0.0f;
		v1 = cameraPos + t1*(vertexPos - cameraPos);
		v2 = cameraPos + t2*(vertexPos - cameraPos);
	
		float3 vDist = v2 - v1;

		float val = length(vDist);

		return val;
	}


	if(t1 < 0.0f && t2 > 1.0f)
	{
		float3 vDist = cameraPos - vertexPos;
		float val = length(vDist);
		return val;
	}


	return 0.0f;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	float fog_val = Check_Sphere(vin.PosL, gCamPos);
	vout.fog_val = fog_val;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	vout.Tex = vin.Tex;

	
    
    return vout;
}


float4 PS(VertexOut pin) : SV_Target
{

	float fog_val = pin.fog_val / 6000.0f;

	float r = fog_val * 0.1f;
	float g = fog_val *0.4f;
	float b = fog_val *0.6f;

	float4 ResColor =  gDiffuseMap.Sample(gsamLinearWrap, pin.Tex);

	ResColor += float4(r, g, b, ResColor.w);

	return ResColor;
}


