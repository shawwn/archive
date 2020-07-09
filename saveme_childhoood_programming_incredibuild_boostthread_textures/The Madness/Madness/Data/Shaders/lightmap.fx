// An empty material, which simply transforms the vertex and sets the color to white.

//------------------------------------
float4x4 WorldViewProj : WorldViewProjection;

//------------------------------------
struct vertexInput {
    float3 Position	: POSITION;
   	float3 Normal : NORMAL;
   	float2 DecalCoord : TEXCOORD0;
   	float2 LightmapCoord : TEXCOORD1;
   	float4 Diffuse : COLOR0;
};

struct vertexOutput {
    float4 HPosition : POSITION;
   	float2 DecalCoord : TEXCOORD0;
   	float2 LightmapCoord : TEXCOORD1;
    float4 Diffuse : COLOR0;
};

//------------------------------------
vertexOutput VS_Lightmap(vertexInput IN) 
{
    vertexOutput OUT;
    OUT.HPosition = mul( float4(IN.Position.xyz , 1.0) , WorldViewProj);
    OUT.DecalCoord = IN.DecalCoord;
    OUT.LightmapCoord = IN.LightmapCoord;
    OUT.Diffuse = IN.Diffuse;
    return OUT;
}

//-----------------------------------
technique textured
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 VS_Lightmap();
		
		ColorArg1[0] = Texture;
		ColorOp[0] = SelectArg1;
		ColorArg1[1] = Texture;
		ColorArg2[1] = Current;
		ColorOp[1] = Modulate;
    }
}