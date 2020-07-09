#ifndef CLIENTSETTINGSINTERFACE_H
#define CLIENTSETTINGSINTERFACE_H

#include "SettingsInterface.h"

#define NOMINMAX
// Application-specific headers to define types
#include <windows.h>
#include <d3dx9.h>

class ClientSettingsInterface : public SettingsInterface
{
private:
	void SetDefaults();
	bool SetApplicationVariable(const std::string &Variable, const std::string &Value);
public:

	// API-Specific Variables
	bool DeviceDestroyed;

	// Application-specific Variables
	HINSTANCE hInstance;
	HWND hWnd;
	bool Windowed;
	UINT Width;
	UINT Height;
	bool Done;
	UINT Adapter;
	D3DDEVTYPE DeviceType;
	UINT DisplayBits;
	UINT DepthBits;
	DWORD VertexProcessing;
	bool DebugVertexShaders;

	bool UseDynamicLighting;
	UINT NumDynamicLights;

	D3DXVECTOR3 ParticleResponse;

	UINT PlaneSize;
	UINT FireworksParticleSize;
	UINT NumFireworksParticles;

	float FireworksParticleDensity;
	UINT FireworksParticleGroup;
	float FireworksParticleLinearDampening;
	float FireworksParticleAngularDampening;
	float FireworksParticleSphereRadius;
	float FireworksParticleLife;

	float FireworksShellDensity;
	UINT FireworksShellGroup;
	float FireworksShellLinearDampening;
	float FireworksShellAngularDampening;
	float FireworksShellSphereRadius;
	float FireworksShellInitialVelocity;
	bool FireworksShellChain;

	float XRot;
	float YRot;
	float Rate;
	UINT Particles;

	std::string ServerAddress;
	Word ServerPort;

	std::string Username;
	std::string Password;
};

extern ClientSettingsInterface gSettings;

#endif