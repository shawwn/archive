// An empty material, which simply transforms the vertex and sets the color to white.

//------------------------------------
float4x4 matWVP : WorldViewProjection;

float MapHeight
<
	string UIName = "MapHeight";
> = 255.0f;

//------------------------------------
struct vertexInput {
    float3 Position	: POSITION;
};

struct vertexOutput {
    float4 HPosition : POSITION;
    float4 Diffuse : COLOR0;
};

//------------------------------------
vertexOutput VS_TransformDiffuse(vertexInput IN) 
{
    vertexOutput OUT;
    OUT.HPosition = mul( float4(IN.Position.xyz , 1.0) , matWVP);
    float Col = IN.Position.y/MapHeight;
    OUT.Diffuse = float4(Col,Col,Col,1.0f);
    return OUT;
}

//-----------------------------------
technique textured
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 VS_TransformDiffuse();
		
		// Just use the color
		ColorArg1[0] = Diffuse;
		AlphaArg1[0] = Diffuse;
		ColorOp[0] = SelectArg1;
		AlphaOp[1] = SelectArg1;
    }
}