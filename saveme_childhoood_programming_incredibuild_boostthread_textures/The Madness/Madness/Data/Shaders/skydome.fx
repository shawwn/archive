// An empty material, which simply transforms the vertex and sets the color to white.

//------------------------------------
float4x4 matWVP : WorldViewProjection;

texture decal;

sampler decalSampler = sampler_state
{
	texture = <decal>;
	MAGFILTER = linear;
	MINFILTER = linear;
	MIPFILTER = linear;
	AddressU = wrap;
	AddressV = wrap;
	AddressW = wrap;
};

//------------------------------------
struct vertexInput {
    float3 Position	: POSITION;
    float2 decalCoord : TEXCOORD0;
};

struct vertexOutput {
    float4 HPosition : POSITION;
    float4 Diffuse : COLOR0;
    float2 decalCoord : TEXCOORD0;
};

//------------------------------------
vertexOutput VS_TransformDiffuse(vertexInput IN) 
{
    vertexOutput OUT;
    IN.Position.yz = float2(IN.Position.z - 2000.0f,IN.Position.y);
    OUT.HPosition = mul( float4(IN.Position.xyz , 1.0) , matWVP);
    OUT.Diffuse.xyzw = 1.0f;;
    OUT.decalCoord = IN.decalCoord;
    return OUT;
}

struct pixelOutput { 
	float4 Color : COLOR0;
};

pixelOutput PS_Textured(vertexOutput IN)
{
	pixelOutput OUT;
	OUT.Color = tex2D(decalSampler,IN.decalCoord);
	return OUT;
}

//-----------------------------------
technique textured
{
    pass p0 
    {		
    	AlphaBlendEnable = false;
		VertexShader = compile vs_1_1 VS_TransformDiffuse();
		PixelShader = compile ps_1_1 PS_Textured();
    }
}