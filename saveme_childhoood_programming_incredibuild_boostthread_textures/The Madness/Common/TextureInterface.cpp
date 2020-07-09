#include "TextureInterface.h"
#include "Parsing.h"
#include "Direct3DInterface.h"
#include "EnumStringInterface.h"
using namespace std;

TextureInterface gTextures;

TDATA::~TDATA()
{
	SAFE_RELEASE(Texture);
}

inline bool ConvertToPoints(string CutterData, VPOINT2 &TopLeft, VPOINT2 &LowerRight) 
{
	size_t Pos;
	string strTopLeft, strLowerRight;
	// Parenthesis are there for animations, not implemented yet, remove parenthesis
	if(CutterData[0] == '(')
		CutterData = CutterData.substr(1,CutterData.length());
	if(CutterData[CutterData.length()-1] == ')')
		CutterData = CutterData.substr(0,CutterData.length()-1);

	Pos = CutterData.find_first_of(',');
	if(BADPOS(Pos))
		return false;

	strTopLeft = Trim(CutterData.substr(0,Pos));
	strLowerRight = Trim(CutterData.substr(Pos+1,CutterData.length()));
	
	if(!IsNumerical(strTopLeft,PARSING_ALLOWWHITESPACE) || !IsNumerical(strLowerRight,PARSING_ALLOWWHITESPACE))
		return false;

    Pos = strTopLeft.find_first_of(' ');
	if(BADPOS(Pos))
		return false;

	TopLeft.X = atoi(Trim(strTopLeft.substr(0,Pos)).c_str());
	TopLeft.Y = atoi(Trim(strTopLeft.substr(Pos+1,strTopLeft.length())).c_str());

	Pos = strLowerRight.find_first_of(' ');
	if(BADPOS(Pos))
		return false;

	LowerRight.X = atoi(Trim(strLowerRight.substr(0,Pos)).c_str());
	LowerRight.Y = atoi(Trim(strLowerRight.substr(Pos+1,strLowerRight.length())).c_str());

	return true;
}

bool TextureInterface::LoadCutterFile(const string &Filename, LPTDATA TextureData, vector<string> &CutterNames)
{
	ifstream in(Filename.c_str());

	if(!in.is_open())
		return false;

	string Line;
	string Name;
	string CutterData;
	VPOINT2 CoordTopLeft, CoordLowerRight;
	size_t Pos;
	while(!in.eof()) {
		getline(in,Line,'\n');
		Line = Trim(Line);
		
		if(!Line.length())
			continue;
		
		Pos = Line.find_last_of("=");
		if(BADPOS(Pos))
			continue;
		Name = Trim(Line.substr(0,Pos));
		CutterData = Trim(Line.substr(Pos+1,Line.length()));
		if(!ConvertToPoints(CutterData,CoordTopLeft,CoordLowerRight))
			continue;

		TextureData->TexCoords.push_back(VTEXCOORD2(CoordTopLeft.X / (float)TextureData->Info.Width,CoordTopLeft.Y / (float)TextureData->Info.Height));
		TextureData->TexCoords.push_back(VTEXCOORD2(CoordLowerRight.X / (float)TextureData->Info.Width,CoordLowerRight.Y / (float)TextureData->Info.Height));
		TextureData->TextureNames.insert(pair<int,int>(Hash(Upper(Name)),(int)TextureData->TexCoords.size()/2));
		CutterNames.push_back(Upper(Name));
	}

	in.close();

	return true;
}

UINT TextureInterface::LoadMultiple(std::string Filename, std::vector<std::string> &CutterNames, D3DCOLOR ColorKey, D3DPOOL Pool, D3DFORMAT Format)
{
	UINT Index;
	if(Index = Load(Filename,0,Pool,0,0,0,Format) == -1)
		return -1;

	Filename = Filename.substr(0,Filename.find_last_of('.')) + ".cutter";
	if(!FilenameIsValid(Filename))
		return false;

	if(!LoadCutterFile(Filename,mTextures[Index],CutterNames))
		gLog << "TextureInterface Texture[" << Filename << "] WARNING: Could not load cutter file.\n";
	return Index;
}

UINT TextureInterface::Load(const string &Filename, D3DCOLOR ColorKey, D3DPOOL Pool, DWORD Usage, UINT Width, UINT Height, D3DFORMAT Format)
{
	if(!mUseDevice) {
		gLog ^ "TextureInterface ERROR: Could not load [" ^ Filename ^ "], device is null.\n";
		return -1;
	}

	if(!mBlankTexture) {
		mBlankTexture = new TDATA;
		if(FAILED(D3DXCreateTextureFromFileEx(mUseDevice,"Data/Textures/debug64x64.JPG",
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &mBlankTexture->Info, NULL,
			&mBlankTexture->Texture))) {
			SAFE_DELETE(mBlankTexture);
		}
		else
		{
			mBlankTexture->Filename = "Data/Textures/debug64x64.JPG";
			mBlankTexture->Pool = D3DPOOL_MANAGED;
		}
	}
	
	Pool = ((Usage == D3DUSAGE_DYNAMIC || Usage == D3DUSAGE_RENDERTARGET) ? D3DPOOL_DEFAULT : Pool);
	if(Format != gD3D.GetMaxSupportedTextureFormat()) {
		if(!gD3D.SupportsTextureFormat(Format,Usage,D3DRTYPE_TEXTURE))
			Format = gD3D.GetMaxSupportedTextureFormat();
	}

	LPTDATA Data = new TDATA;
	HRESULT hr;
	// If filename isn't "", load the texture from file, else create a blank texture.
	if(Filename.length()) {
		if(!FilenameIsValid(Filename)) { // The filename isn't "", but we can't find the file.
			gLog << "TextureInterface ERROR: Could not load [" << Filename << "], filename is not a valid file.\n";
			return -1;
		}
		if(FAILED(hr = D3DXCreateTextureFromFileEx(mUseDevice, Filename.c_str(), 
						(Width == 0 ? D3DX_DEFAULT : Width), 
						(Height == 0 ? D3DX_DEFAULT : Height),
						D3DX_DEFAULT, // MipLevels (create complete chain)
						Usage, // 0 (Normal), Dynamic, or RenderTarget
						D3DFMT_UNKNOWN, // Texture Format, take it from file or max supported
						Pool, // Memory pool.  Dynamic or render target textures go into video memory and get reallocated when the device is reset.
						D3DX_DEFAULT, D3DX_DEFAULT, // Filters
						ColorKey,
						&Data->Info, // File info
						NULL,  // Palette, always NULL
						&Data->Texture))) 
		{
			gLog ^ "TextureInterface ERROR: Could not create texture with Filename[" ^ Filename ^ "] Width[" ^ Width ^ "] Height[" ^ Height ^ "] Format[" ^ EnumToString.Find(Format,"D3DFMT") ^ "] Pool[" ^ EnumToString.Find(Pool,"D3DPOOL") ^ "] Usage[" ^ EnumToString.Find(Usage,"D3DUSAGE") ^ ", D3DXCreateTextureFromFile failed with error [" ^ EnumToString.Find(hr,"D3DERR","E") ^ "]\n";
			return -1;
		}
		Data->Filename = Filename;
	}
	else // Create a blank texture
	{
		if(FAILED(hr = D3DXCreateTexture(mUseDevice, 
						(Width == 0 ? 128 : Width),
						(Height == 0 ? 128 : Height),
						D3DX_DEFAULT, // MipLevels
						Usage,
						Format,
						Pool,
						&Data->Texture)))
		{
			gLog ^ "TextureInterface ERROR: Could not create texture with Width[" ^ Width ^ "] Height[" ^ Height ^ "] Format[" ^ EnumToString.Find(Format,"D3DFMT") ^ "] Pool[" ^ EnumToString.Find(Pool,"D3DPOOL") ^ "] Usage[" ^ EnumToString.Find(Usage,"D3DUSAGE") ^ "]\n";
			return -1;
		}
	}

	Data->Usage = Usage;
	Data->Pool = Pool;
	Data->ColorKey = ColorKey;

	mTextures.push_back(Data);
	return NumTextures()-1;
}

void TextureInterface::OnDestroyDevice()
{
	for(UINT i = 0; i < NumTextures(); i++) {
		LPTDATA Texture = mTextures[i];
		SAFE_RELEASE(Texture->Texture);
	}
}

void TextureInterface::OnLostDevice()
{
	for(UINT i = 0; i < NumTextures(); i++) {
		LPTDATA Texture = mTextures[i];
		if(Texture->Usage & D3DUSAGE_DYNAMIC || Texture->Pool == D3DPOOL_DEFAULT)
			SAFE_RELEASE(Texture->Texture);
	}
}

bool TextureInterface::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	if(!Device) { 
		gLog ^ "TextureInterface ERROR: Reset() failed, device is null.\n";
		return false;
	}
	mUseDevice = Device;
	HRESULT hr;
	for(UINT i = 0; i < NumTextures(); i++) { 
		LPTDATA Texture = mTextures[i];
		if(!Texture->Texture)
		{
			if(FAILED(hr = D3DXCreateTextureFromFileEx(mUseDevice, Texture->Filename.c_str(),
											D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
											Texture->Usage,
											D3DFMT_UNKNOWN,
											Texture->Pool,
											D3DX_DEFAULT, D3DX_DEFAULT,
											Texture->ColorKey,
											&Texture->Info,
											NULL,
											&Texture->Texture))) 
			{
				LogDxErr(hr,"TextureInterface::OnResetDevice(D3DXCreateTextureFromFileEx)");
			}
		}
		/*if( (Texture->Pool == D3DPOOL_DEFAULT) || (Texture->Usage == D3DUSAGE_RENDERTARGET) || (Texture->Usage == D3DUSAGE_DYNAMIC) || Recreated ) {
			//SAFE_RELEASE(Texture->Texture);
			if(FAILED(hr = D3DXCreateTextureFromFile(mUseDevice, Texture->Filename.c_str(), 
						&Texture->Texture))) {
				LogDxErr(hr,"D3DXCreateTextureFromFile");
				gLog << "TextureInterface ERROR: Could not reload texture [" << Texture->Filename << "]\n";
				SAFE_DELETE(Texture);
			}
			else
			{
				gLog << "TextureInterface: Reloaded texture [" << Texture->Filename << "]\n";
			}
		}*/
	}
	return true;
}

void TextureInterface::Destroy()
{
	SAFE_DELETE(mBlankTexture);
	for(UINT i = 0; i < NumTextures(); i++) 
		SAFE_DELETE(mTextures[i]);
	mUseDevice = NULL;
}