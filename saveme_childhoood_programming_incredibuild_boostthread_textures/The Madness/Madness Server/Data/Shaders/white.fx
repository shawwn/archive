// An empty material, which simply transforms the vertex and sets the color to white.

//------------------------------------
float4x4 WorldViewProj : WorldViewProjection;

//------------------------------------
struct AppInput {
    float3 Position	: POSITION;
    float4 Diffuse : COLOR0;
};

struct VertexOutput {
    float4 HPosition : POSITION;
    float4 Diffuse : COLOR0;
};

//------------------------------------
VertexOutput VS_TransformDiffuse(AppInput IN) 
{
    VertexOutput OUT;
    OUT.HPosition = mul( float4(IN.Position.xyz , 1.0) , WorldViewProj);
    OUT.Diffuse = IN.Diffuse;
    return OUT;
}

float4 PS_TransformDiffuse(VertexOutput IN) : Color
{
	return float4(IN.Diffuse.xyz,1.0f);
}



//-----------------------------------
technique textured
{
	
    pass p0 
    {		
    	cullmode = none;
		VertexShader = compile vs_1_1 VS_TransformDiffuse();
		
		PixelShader = compile ps_1_1 PS_TransformDiffuse();
    }
}