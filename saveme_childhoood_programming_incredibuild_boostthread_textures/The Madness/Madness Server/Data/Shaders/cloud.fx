// An empty material, which simply transforms the vertex and sets the color to white.

//------------------------------------
float4x4 matWVP : WorldViewProjection;


texture Decal
<
	string ResourceName = "../Textures/cloud.jpg";
	string ResourceType = "2D";
>;

sampler2D TexSampler = sampler_state
{
	Texture = <Decal>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//------------------------------------
struct vertexInput {
    float3 Position	: POSITION;
    float2 Coord : TEXCOORD0;
    //float2 Size : TEXCOORD1;
};
 
struct vertexOutput {
    float4 HPosition : POSITION;
    float2 Coord : TEXCOORD0;
    float Diffuse : COLOR0;
        //float Size : PSIZE0;
};

struct pixelInput {
	float2 UV : TEXCOORD0;
};

//------------------------------------
vertexOutput VS_TransformDiffuse(vertexInput IN) 
{
    vertexOutput OUT;
    OUT.HPosition = mul( float4(IN.Position.xyz , 1.0) , matWVP);
   	//OUT.Diffuse = float4(1,clamp(1.0f-Temp.y,0,1),clamp(1.0f-Temp.y,0,1),1);
	OUT.Coord = IN.Coord;
	OUT.Diffuse = 1.0f;
    //OUT.Diffuse = float4(IN.Size,IN.Size,IN.Size,1.0f);
    //OUT.Diffuse = float4(IN.Size,IN.Size,IN.Size,1.0f);
    //OUT.Size = IN.Size;
    
    return OUT;
}

float4 PS_Textured(pixelInput IN) : COLOR
{
	return float4(tex2D(TexSampler,IN.UV).rgba);
}

//-----------------------------------
technique textured
{
    pass p0 
    {		
    	AlphaBlendEnable = true;
    	SrcBlend = srccolor;
    	DestBlend = one;
		VertexShader = compile vs_1_1 VS_TransformDiffuse();
		//PixelShader = compile ps_1_1 PS_Textured();
		
		Texture[0] = <Decal>;
  		MinFilter[0] = Linear;
  		MagFilter[0] = Linear;
  		MipFilter[0] = Linear;

        ColorOp[0]	 = Modulate;
        ColorArg1[0] = Diffuse;
        ColorArg2[0] = Texture;
        //AlphaOp[0]   = Modulate;
        //AlphaArg1[0] = Diffuse;
        //AlphaArg2[0] = Texture;
    }
}