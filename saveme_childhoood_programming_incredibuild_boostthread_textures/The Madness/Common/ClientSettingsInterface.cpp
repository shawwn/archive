#include "ClientSettingsInterface.h"
#include "LoggingInterface.h"
#include "Parsing.h"
#include "Macros.h"

ClientSettingsInterface gSettings;

void ClientSettingsInterface::SetDefaults()
{
	DeviceDestroyed = false;

	hInstance = NULL;
	hWnd = NULL;
	Windowed = false;
	Width = 1024;
	Height = 768;
	Done = false;
	Adapter = 0;
	DeviceType = D3DDEVTYPE_HAL;
	DisplayBits = 0;
	DepthBits = 0;
	VertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	DebugVertexShaders = false;
	UseDynamicLighting = true;
	NumDynamicLights = 5;

	Particles = 15;
	XRot = 0;
	YRot = 0;
	Rate = 1.0f;
	
	ParticleResponse = D3DXVECTOR3(0,0,0);

	FireworksParticleSize = 500;
	NumFireworksParticles = 100;
	PlaneSize = 10000;

	FireworksParticleDensity = 1.0f;
	FireworksParticleGroup = 2;
	FireworksParticleLinearDampening = 0.0f;
	FireworksParticleAngularDampening = 5.0f;
	FireworksParticleSphereRadius = 5.0f;
	FireworksParticleLife = 20.0f;

	FireworksShellDensity = 1.0f;
	FireworksShellGroup = 2;
	FireworksShellLinearDampening = 0.0f;
	FireworksShellAngularDampening = 5.0f;
	FireworksShellSphereRadius = 5.0f;
	FireworksShellInitialVelocity = 150.0f;
	FireworksShellChain = false;

	ServerAddress = "127.0.0.1";
	ServerPort = 31337;

	Username = "Test";
	Password = "Failed";

}

bool ClientSettingsInterface::SetApplicationVariable(const std::string &Variable, const std::string &Value)
{
	AddBool(Variable,"WINDOWED",Windowed,Value);
	AddUINT(Variable,"WIDTH",Width,Value);
	AddUINT(Variable,"HEIGHT",Height,Value);
	AddUINT(Variable,"ADAPTER",Adapter,Value);

	if(Variable == "DEVICETYPE")
	{
		std::string UpVal = Upper(Value);
		if(UpVal == "HARDWARE") 
			DeviceType = D3DDEVTYPE_HAL;
		if(UpVal == "SOFTWARE") 
			DeviceType = D3DDEVTYPE_SW;
		if(UpVal == "REFERENCE" || Value == "REF") 
			DeviceType = D3DDEVTYPE_REF;
	}
	if(Variable == "FORCEVERTEXPROCESSING") {
		std::string UpVal = Upper(Value);
		if(UpVal == "HARDWARE")
			VertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		if(UpVal == "SOFTWARE")
			VertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		if(UpVal == "MIXED")
			VertexProcessing = D3DCREATE_MIXED_VERTEXPROCESSING;
	}
	if(Variable == "FORCEDISPLAYMODEBITS") { DisplayBits = (Value == "16" || Value == "32" ? atoi(Value.c_str()) : 0); }
	if(Variable == "FORCEDEPTHBUFFERBITS") { DepthBits = (Value == "16" || Value == "24" ? atoi(Value.c_str()) : 0); }
	
	AddBool	(Variable,"DEBUGVERTEXSHADERS",DebugVertexShaders,Value);
	AddFloat(Variable,"RATE",Rate,Value);
	AddUINT	(Variable,"PARTICLES",Particles,Value);
	AddFloat(Variable,"XROT",XRot,Value);
	AddFloat(Variable,"YROT",YRot,Value);
	AddBool	(Variable,"USEDYNAMICLIGHTING",UseDynamicLighting,Value);
	AddUINT	(Variable,"NUMDYNAMICLIGHTS",NumDynamicLights,Value);
	AddFloat(Variable,"PARTICLERESPONSEX",ParticleResponse.x,Value);
	AddFloat(Variable,"PARTICLERESPONSEY",ParticleResponse.y,Value);
	AddFloat(Variable,"PARTICLERESPONSEZ",ParticleResponse.z,Value);
	AddUINT	(Variable,"PLANESIZE",PlaneSize,Value);
	AddUINT	(Variable,"FIREWORKSPARTICLESIZE",FireworksParticleSize,Value);
	AddUINT	(Variable,"NUMFIREWORKSPARTICLES",NumFireworksParticles,Value);
	AddFloat(Variable,"FIREWORKSSHELLDENSITY",FireworksShellDensity,Value);
	AddUINT	(Variable,"FIREWORKSSHELLGROUP",FireworksShellGroup,Value);
	AddFloat(Variable,"FIREWORKSSHELLANGULARDAMPENING",FireworksShellAngularDampening,Value);
	AddFloat(Variable,"FIREWORKSSHELLLINEARDAMPENING",FireworksShellLinearDampening,Value);
	AddFloat(Variable,"FIREWORKSSHELLSPHERERADIUS",FireworksShellSphereRadius,Value);
	AddFloat(Variable,"FIREWORKSSHELLINITIALVELOCITY",FireworksShellInitialVelocity,Value);
	AddBool	(Variable,"FIREWORKSSHELLCHAIN",FireworksShellChain,Value);
	AddFloat(Variable,"FIREWORKSPARTICLEDENSITY",FireworksParticleDensity,Value);
	AddUINT	(Variable,"FIREWORKSPARTICLEGROUP",FireworksParticleGroup,Value);
	AddFloat(Variable,"FIREWORKSPARTICLELINEARDAMPENING",FireworksParticleLinearDampening,Value);
	AddFloat(Variable,"FIREWORKSPARTICLEANGULARDAMPENING",FireworksParticleAngularDampening,Value);
	AddFloat(Variable,"FIREWORKSPARTICLESPHERERADIUS",FireworksParticleSphereRadius,Value);
	AddFloat(Variable,"FIREWORKSPARTICLELFIE",FireworksParticleLife,Value);
	AddString(Variable,"SERVERADDRESS",ServerAddress,Value);
	AddWord	(Variable,"SERVERPORT",ServerPort,Value);
	AddString(Variable,"USERNAME",Username,Value);
	AddString(Variable,"PASSWORD",Password,Value);

	return false;
}