// An empty material, which simply transforms the vertex and sets the color to white.

//------------------------------------
float4x4 matWVP : WorldViewProjection;

texture alphaTexture;
texture decalTexture;

sampler alphaSampler = sampler_state
{
	texture = <alphaTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

sampler decalSampler = sampler_state
{
	texture = <decalTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

struct LightDesc
{
	float3 lightColor;
	float  lightIntensity;
};

LightDesc Light = {
 {0.0f, 0.0f, 1.0f},
 { 1.0f }
 };

//------------------------------------
struct vertexInput {
    float3 Position	: POSITION;
    float2 Coord : TEXCOORD0;
};

struct vertexOutput {
    float4 HPosition : POSITION;
    float4 Diffuse : COLOR0;
    float2 Coord : TEXCOORD0;
};

struct pixelOutput {
	float4 Color : COLOR0;
};

//------------------------------------
vertexOutput VS_TransformDiffuse(vertexInput IN) 
{
    vertexOutput OUT;
    OUT.HPosition = mul( float4(IN.Position.xyz , 1.0) , matWVP);
    OUT.Diffuse = float4(1.0f, 1.0f,1.0f,1.0f);
    OUT.Coord = IN.Coord;
    return OUT;
}

pixelOutput PS_Lit(vertexOutput IN,
				uniform LightDesc Light)
{
	pixelOutput OUT;
	OUT.Color.xyz = tex2D(alphaSampler,IN.Coord) *
					tex2D(decalSampler,IN.Coord) *
					Light.lightColor.xyz *
					Light.lightIntensity*Light.lightIntensity;
	OUT.Color.w = tex2D(alphaSampler,IN.Coord);
	return OUT;
}

//-----------------------------------
technique textured
{
    pass p0 
    {	
    	AlphaBlendEnable = true;
    	srcblend = one;
    	destblend = one;	
		VertexShader = compile vs_1_1 VS_TransformDiffuse();
		PixelShader = compile ps_2_0 PS_Lit(Light);
    }
}