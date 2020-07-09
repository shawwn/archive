// An empty material, which simply transforms the vertex and sets the color to white.

//------------------------------------
float4x4 matWVP : WorldViewProjection;

struct LightDesc
{
	float3 lightPos;
	float3 lightColor;
	float  lightDecayDistance;
	float3 lightDecayAttributes;
	float  lightIntensity;
};

LightDesc Light = {
 { 0.0f, 1.0f, 0.0f },
 {1.0f, 1.0f, 1.0f},
 { 32.0f },
 { 0.0f, 0.5f, 0.0f },
 { 1.0f }
 };
 
float3 globalAmbient = { 1.0f, 1.0f, 1.0f };
//float3 eyePos = {0.0f,0.0f,0.0f};

float gridSize : SpecularPower
<
	string UIName = "Grid Size";
> = { 32.0f };

texture decalTexture;

sampler decalSampler = sampler_state
{
	texture = <decalTexture>;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

//------------------------------------
struct vertexInput {
    float3 Position	: POSITION;
};

struct ambientVertexOutput {
    float4 HPosition : POSITION;
    float4 Ambient : COLOR0;
    float2 decalCoord : TEXCOORD0;	
};

struct vertexOutput {
    float4 HPosition : POSITION;
    float2 decalCoord : TEXCOORD0;
    float4 objectPos : TEXCOORD1;
};

struct pixelOutput {
	float4 Diffuse : COLOR0;
};

//------------------------------------

ambientVertexOutput VS_Ambient(vertexInput IN)
{
	ambientVertexOutput OUT;
	OUT.HPosition = mul(float4(IN.Position.xyz,1), matWVP);
	OUT.Ambient = float4(globalAmbient.xyz,1.0f);
    OUT.decalCoord.x = -IN.Position.x / gridSize;
    OUT.decalCoord.y = IN.Position.z / gridSize;
    return OUT;
}

vertexOutput VS_Lit(vertexInput IN) 
{
    vertexOutput OUT;
    OUT.HPosition = mul( float4(IN.Position.xyz , 1) ,  matWVP);
    OUT.objectPos = float4(IN.Position.xyz,1);
    OUT.decalCoord.x = -IN.Position.x / gridSize;
    OUT.decalCoord.y = IN.Position.z / gridSize;
    return OUT;
}

pixelOutput PS_Ambient(ambientVertexOutput IN)
{
	pixelOutput OUT;
	OUT.Diffuse = tex2D(decalSampler,IN.decalCoord) * IN.Ambient;
	return OUT;
}

pixelOutput PS_Lit(vertexOutput IN,
		   uniform LightDesc Light)
{
	pixelOutput OUT;
	float Distance;
	float Attenuation;
	float3 Diffuse;
	Distance = distance(Light.lightPos,IN.objectPos.xyz) / Light.lightDecayDistance;
	Attenuation = 1 / (Light.lightDecayAttributes.x + 
						Light.lightDecayAttributes.y * Distance +
						Light.lightDecayAttributes.z * (Distance*Distance));
	Diffuse.xyz = Light.lightColor.xyz * Light.lightIntensity * Attenuation;
	
	OUT.Diffuse.xyz = Diffuse.xyz * tex2D(decalSampler,IN.decalCoord).xyz;
	OUT.Diffuse.w = 1.0f;
	return OUT;
}



//-----------------------------------
technique textured
{
    pass p0 
    {		
    	AlphaBlendEnable = true;
    	ZWriteEnable = true;
    	SrcBlend = one;
    	DestBlend = zero;
    	
		VertexShader = compile vs_1_1 VS_Ambient();
		PixelShader = compile ps_1_1 PS_Ambient();
    }
    
    pass p1
    {
		AlphaBlendEnable = true;
    	SrcBlend = one;
    	DestBlend = one;
    	//ZFunc = equal;
    	
    	ZWriteEnable = false;
    	
    	VertexShader = compile vs_1_1 VS_Lit();
    	PixelShader = compile ps_2_0 PS_Lit(Light);
    }
}