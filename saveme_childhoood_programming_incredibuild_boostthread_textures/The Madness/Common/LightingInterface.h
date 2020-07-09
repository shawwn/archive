#ifndef LIGHTINGINTERFACE_H
#define LIGHTINGINTERFACE_H

#include <list>
#include <d3dx9.h>
#include "Macros.h"

class Light
{
public:
	Light() : Position(D3DXVECTOR3(0,0,0)), Intensity(1.0f), Color(D3DXVECTOR3(0.92f,0.95f,1.0f)), DecayDistance(40), DecayAttributes(D3DXVECTOR3(0,0,1)) { }
	Light(D3DXVECTOR3 Pos, float Intens, D3DXVECTOR3 Col, float DecayDist, D3DXVECTOR3 DecayAttr) : Position(Pos), Intensity(Intens), Color(Col), DecayDistance(DecayDist), DecayAttributes(DecayAttr) { }
	D3DXVECTOR3 Position;
	float Intensity;
	D3DXVECTOR3 Color;
	float DecayDistance;
	D3DXVECTOR3 DecayAttributes;
};

class LightingInterface
{
private:
	std::list<Light *> mLights;
	int mNumLights;
public:
	typedef std::list<Light *>::iterator Iterator;

	LightingInterface() : mNumLights(0) { }
	~LightingInterface() {
		for(Iterator it = Begin(); it != End(); it++) 
			SAFE_DELETE(*it); }

	inline Light *AddLight(D3DXVECTOR3 Position, float Intensity = 1.0f, D3DXVECTOR3 Color = D3DXVECTOR3(1,1,1), float DecayDistance = 40.0f, D3DXVECTOR3 DecayAttributes = D3DXVECTOR3(0,0,1)) {
		Light *l = new Light(Position,Intensity, Color,DecayDistance,DecayAttributes);
		mLights.push_back(l);
		mNumLights++;
		return l;
	}

	inline bool ReleaseLight(Light *l) { 
		for(Iterator it = Begin(); it != End(); it++) {
			if((*it) == l) {
				SAFE_DELETE(*it);
				mLights.erase(it);
				mNumLights--;
				return true;
			}
		}
		return false;
	}

	inline bool ReleaseLight(UINT Index) {
		UINT i;
		Iterator it;
		for(it = Begin(), i = 0; it != End(); it++, i++) {
			if(i == Index)
			{
				SAFE_DELETE(*it);
				mLights.erase(it);
				mNumLights--;
				return true;
			}
		}
		return false;
	}
	
	inline Iterator Begin() { return mLights.begin(); }
	inline Iterator End() { return mLights.end(); }
	inline std::list<Light *> &Lights() { return mLights; }
	inline int LightCount() const { return mNumLights; }
};

extern LightingInterface gLights;

#endif