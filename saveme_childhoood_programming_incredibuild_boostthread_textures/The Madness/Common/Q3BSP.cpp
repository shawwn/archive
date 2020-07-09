#include "Q3BSP.h"
#include "Parsing.h"
#include "LoggingInterface.h"
#include "TextureInterface.h"
using namespace std;

inline void Swap(float &First, float &Second) {
	float Temp = First;
	First = Second;
	Second = Temp;
}

bool Q3BSP::Load(const std::string &Filename)
{
	gLog << "Q3BSP: Loading [" << Filename <<"]...\n";
	if(!FilenameIsValid(Filename)) {
		gLog << "Q3BSP ERROR: Filename [" << Filename << "] is not valid.\n";
		return false;
	}
	
	mFilename = Filename;

	ifstream in(Filename.c_str(),ios::binary);
	if(!in.is_open()) {
		gLog << "Q3BSP ERROR: Could not open BSP file [" << Filename << "].\n";
		return false;
	}	

	in.read((char *)&mHeader,sizeof(Q3Header));
	
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Textures>(in, mTextures,mNumTextures,1) << "] textures.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Planes>(in, mPlanes,mNumPlanes,2) << "] planes.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Nodes>(in, mNodes,mNumNodes,3) << "] nodes.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Leafs>(in, mLeafs,mNumLeafs,4) << "] leafs.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3LeafFaces>(in, mLeafFaces,mNumLeafFaces,5) << "] leaf faces.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3LeafBrushes>(in, mLeafBrushes,mNumLeafBrushes,6) << "] leaf brushes.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Models>(in, mModels,mNumModels,7) << "] models.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Brushes>(in, mBrushes,mNumBrushes,8) << "] brushes.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3BrushSides>(in, mBrushSides,mNumBrushSides,9) << "] brush sides.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Vertices>(in, mVertices,mNumVertices,10) << "] vertices.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3MeshVertices>(in, mMeshVertices,mNumMeshVertices,11) << "] mesh vertices.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Effects>(in, mEffects,mNumEffects,12) << "] effects.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Faces>(in, mFaces,mNumFaces,13) << "] faces.\n";
	gLog << "Q3BSP [" << Filename << "]:  Loaded [" << InitPart<Q3Lightmaps>(in, mLightmaps,mNumLightmaps,14) << "] lightmaps.\n";

	gLog << "Q3BSP [" << Filename << "]: Loaded.  Fixing position data...\n";
	in.close();
	in.clear();

	UINT i;
	for(i = 0; i < mNumVertices; i++) {
		Swap(mVertices[i].Position[1],mVertices[i].Position[2]);
		Swap(mVertices[i].Normal[1],mVertices[i].Normal[2]);
		mVertices[i].Position[1] += 0.5f;
	}

	gLog << "Q3BSP [" << Filename << "]: Creating vertex indices...\n";
	mVertexIndices = new unsigned short [(mNumVertices-2)*3];

	int j;
	UINT k = 0;
	for(i = 0; i < mNumFaces; i++) {
		for(j = 0; j < mFaces[i].NumVertices-2; j++) {
			mVertexIndices[k  ] = mFaces[i].IndexVertex;
			mVertexIndices[k+1] = mFaces[i].IndexVertex + j + 1;
			mVertexIndices[k+2] = mFaces[i].IndexVertex + j + 2;
			k += 3;
		}
	}


	mTextureIndices = new UINT[mNumTextures];
	memset(mTextureIndices,-1,sizeof(UINT)*mNumTextures);

	string TexName;
	for(i = 0; i < mNumTextures; i++) {
		TexName = (string)mTextures[i].Name;

		if(TexName == "noshader")
			continue;
		if(TexName == "textures/common/caulk") {
			mCaulkIndex = i;
			continue;
		}
		mTextureIndices[i] = gTextures.Load("Data/" + (string)mTextures[i].Name + ".jpg");
		if(mTextureIndices[i] == -1) {
			mTextureIndices[i] = gTextures.Load("Data/" + (string)mTextures[i].Name + ".tga");
		}
	}

	return true;
}

bool Q3BSP::Render(LPDIRECT3DDEVICE9 Device)
{
	Device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	for(UINT i = 0; i < mNumFaces; i++) {
		TDATA *data = gTextures.Find(mTextureIndices[mFaces[i].IndexTexture]);
		Device->SetTexture(0,data->Texture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,mFaces[i].NumVertices-2,(void *)&mVertices[mFaces[i].IndexVertex],sizeof(Q3Vertices));
	}
	//Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLEFAN,0,mNumVertices,mNumVertices-2,mVertexIndices,D3DFMT_INDEX16,mVertices,sizeof(Q3Vertices));

	return true;
}