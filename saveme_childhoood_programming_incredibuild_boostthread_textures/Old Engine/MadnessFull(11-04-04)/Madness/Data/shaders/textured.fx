
texture Decal
<
	string ResourceName = "default_color.dds";
	string ResourceType = "2D";
>;

sampler2D DecalSampler = sampler_state
{
	Texture = <Decal>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//------------------------------------
float4x4 WorldViewProj : WorldViewProjection;

//------------------------------------
struct AppData {
    float3 Position	: POSITION;
    float2 UV : TEXCOORD0;
};

struct VertexData {
    float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
};

//------------------------------------
VertexData VS_Transform(AppData IN) 
{
    VertexData OUT;
    OUT.HPosition = mul( float4(IN.Position.xyz , 1.0) , WorldViewProj);
    OUT.UV = IN.UV;
    return OUT;
}

float4 PS_Textured(VertexData IN) : COLOR
{
	return float4(tex2D(DecalSampler,IN.UV).xyz,1.0f);
}

//-----------------------------------
technique textured
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 VS_Transform();
		
		PixelShader = compile ps_1_1 PS_Textured();
    }
}