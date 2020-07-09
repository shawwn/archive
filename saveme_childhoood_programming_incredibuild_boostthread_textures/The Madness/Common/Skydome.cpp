#include "GameInterface.h"
#include "Skydome.h"
#include "Direct3DInterface.h"
#include "EffectInterface.h"
#include "TextureInterface.h"
#include <math.h>
#include <windows.h>

#define DTOR 0.017453292516666666666666666666667f
#define SQR( x ) x*x
#define PI 3.141592653f

bool Skydome::GenerateDome(float radius, float dtheta, float dphi, float hTile, float vTile)
{
	int theta, phi;

	// Make sure our vertex array is clear
	SkydomeVertex *Vertices = NULL;
	if (Vertices) 
	{
		delete Vertices;
		Vertices = NULL;
		mNumVertices = 0;
	}

	// Initialize our Vertex array
	mNumVertices = (int)((360/dtheta)*(90/dphi)*4);
	Vertices = new SkydomeVertex[mNumVertices];
	ZeroMemory(Vertices, sizeof(SkydomeVertex)*mNumVertices);

	// Used to calculate the UV coordinates
	float vx, vy, vz, mag;

	// Generate the dome
	int n = 0;
	for (phi=0; phi <= 90 - dphi; phi += (int)dphi)
	{
		for (theta=0; theta <= 360 - dtheta; theta += (int)dtheta)
		{
			// Calculate the vertex at phi, theta
			Vertices[n].x = radius * sinf(phi*DTOR) * cosf(DTOR*theta);
			Vertices[n].y = radius * sinf(phi*DTOR) * sinf(DTOR*theta);
			Vertices[n].z = radius * cosf(phi*DTOR);

			// Create a vector from the origin to this vertex
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			// Normalize the vector
			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			// Calculate the spherical texture coordinates
			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			// Calculate the vertex at phi+dphi, theta
			Vertices[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(theta*DTOR);
			Vertices[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(theta*DTOR);
			Vertices[n].z = radius * cosf((phi+dphi)*DTOR);
			
			// Calculate the texture coordinates
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			// Calculate the vertex at phi, theta+dtheta
			Vertices[n].x = radius * sinf(DTOR*phi) * cosf(DTOR*(theta+dtheta));
			Vertices[n].y = radius * sinf(DTOR*phi) * sinf(DTOR*(theta+dtheta));
			Vertices[n].z = radius * cosf(DTOR*phi);
			
			// Calculate the texture coordinates
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			if (phi > -90 && phi < 90)
			{
				// Calculate the vertex at phi+dphi, theta+dtheta
				Vertices[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*(theta+dtheta));
				Vertices[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*(theta+dtheta));
				Vertices[n].z = radius * cosf((phi+dphi)*DTOR);
				
				// Calculate the texture coordinates
				vx = Vertices[n].x;
				vy = Vertices[n].y;
				vz = Vertices[n].z;

				mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
				vx /= mag;
				vy /= mag;
				vz /= mag;

				Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
				Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
				n++;
			}
		}
	}

	// Fix the problem at the seam
	for (UINT i=0; i < mNumVertices-3; i++)
	{
		if (Vertices[i].u - Vertices[i+1].u > 0.9f)
			Vertices[i+1].u += 1.0f;

		if (Vertices[i+1].u - Vertices[i].u > 0.9f)
			Vertices[i].u += 1.0f;

		if (Vertices[i].u - Vertices[i+2].u > 0.9f)
			Vertices[i+2].u += 1.0f;

		if (Vertices[i+2].u - Vertices[i].u > 0.9f)
			Vertices[i].u += 1.0f;

		if (Vertices[i+1].u - Vertices[i+2].u > 0.9f)
			Vertices[i+2].u += 1.0f;

		if (Vertices[i+2].u - Vertices[i+1].u > 0.9f)
			Vertices[i+1].u += 1.0f;

		if (Vertices[i].v - Vertices[i+1].v > 0.8f)
			Vertices[i+1].v += 1.0f;

		if (Vertices[i+1].v - Vertices[i].v > 0.8f)
			Vertices[i].v += 1.0f;

		if (Vertices[i].v - Vertices[i+2].v > 0.8f)
			Vertices[i+2].v += 1.0f;

		if (Vertices[i+2].v - Vertices[i].v > 0.8f)
			Vertices[i].v += 1.0f;

		if (Vertices[i+1].v - Vertices[i+2].v > 0.8f)
			Vertices[i+2].v += 1.0f;

		if (Vertices[i+2].v - Vertices[i+1].v > 0.8f)
			Vertices[i+1].v += 1.0f;
	}

	gBuffers.ReleaseVertexBuffer(mVertexBufferIndex);
	gDeclarations.ReleaseDeclaration(mVertexDeclIndex);

	UINT Stride = sizeof(D3DXVECTOR3)+sizeof(float)*2;

	mVertexBufferIndex = gBuffers.CreateVertexBuffer(mNumVertices*Stride,Stride);
	if(mVertexBufferIndex == -1) {
		gLog << "Skydome ERROR: Could not create a vertex buffer.\n";
		return false;
	}

	LPDIRECT3DVERTEXBUFFER9 VB = gBuffers.AccessVertexBuffer(mVertexBufferIndex);
	HRESULT hr;

	char *ptr = NULL;
	if(FAILED(hr = VB->Lock(0,0,(void**)&ptr,0))) {
		gLog ^ "Skydome ERROR: Failed to lock vertex buffer.\n";
		SAFE_RELEASE(VB);
		return false;
	}

	memcpy(ptr,Vertices,mNumVertices*sizeof(SkydomeVertex));
	VB->Unlock();

	SAFE_DELETE(Vertices);

	D3DVERTEXELEMENT9 Elements[3];
	gDeclarations.Start(Elements).Fill(0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0)
		.Fill(0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0)
		.End();

	mVertexDeclIndex = gDeclarations.CreateDeclaration(Elements);
	if(mVertexDeclIndex == -1) {
		gLog ^ "Skydome ERROR: Failed to create vertex declaration.\n";
		gBuffers.ReleaseVertexBuffer(mVertexBufferIndex);
		return false;
	}

	mEffectIndex = gEffects.Load("Data/Shaders/skydome.fx");
	if(mEffectIndex == -1) {
		gLog ^ "Skydome ERROR: Failed to load skydome.fx.\n";
		return false;
	}

	mTextureIndex = gTextures.Load("Data/Textures/stars.jpg");
	if(mTextureIndex == -1) {
		gLog ^ "Skydome ERROR: Failed to load texture.\n";
		return false;
	}

	return true;
}

bool Skydome::Render(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return false;

	HRESULT hr;

	LPD3DXEFFECT eff = gEffects.FindEffect(mEffectIndex);
	if(eff)
	{
		if(!gDeclarations.SetDeclaration(mVertexDeclIndex))
			return false;
		if(!gBuffers.SetStream(mVertexBufferIndex))
			return false;

		D3DXMATRIX Translation;
		D3DXMatrixTranslation(&Translation,gEngine.GetCamera().GetEye().x,gEngine.GetCamera().GetEye().y,gEngine.GetCamera().GetEye().z);

		eff->SetMatrix("matWVP",&(Translation*gEngine.GetMatVP()));
		eff->SetTexture("decal",gTextures.Find(mTextureIndex)->Texture);
		UINT Passes;
		eff->Begin(&Passes,0);
		eff->BeginPass(0);

		if(FAILED(hr = Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,mNumVertices-2))) {
			LogDxErr(hr,"SkyDome::Device::DrawPrimitive");
			return false;
		}
		eff->EndPass();
		eff->End();
	}

	return true;
}
