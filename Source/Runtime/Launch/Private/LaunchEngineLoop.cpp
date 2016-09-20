#include "../Public/LaunchEngineLoop.h"
#include "HAL/PlatformTLS.h"
#include "RHI.h"
#include "ShaderCore.h"
#include "RenderingThread.h"
#include "EngineGlobals.h"
#include "../../Engine/Classes/Engine/Engine.h"

FEngineLoop::FEngineLoop()
{

}

int32 FEngineLoop::PreInit(int32 ArgC, char* ArgV[], const char* AdditionalCommandline)
{
	//Init LogConsole

	//EnableBackLog
	//GLog->EnableBacklog(true);

	//Multithread------------------------------------------------------------------------------
	//remember thread id of the main thread;
	//GGameThreadId = FPlatformTLS::GetCurrentThreadId();

	//FPlatformProcess::SetThreadAffinityMask(FPlatformAffinity::GetMainGameMask());
	//FPlatformProcess::SetupGameOrRenderThread(false);

	//FTaskGraphInterface::Startup(FPlatformMisc::NumberOfCores());
	//FTaskGraphInterface::Get().AttachToThread(ENamedThreads::GameThread);
	//Multithread------------------------------------------------------------------------------
	LoadCoreModules();

	//Multithread------------------------------------------------------------------------------
	//if (FPlatformProcess::SupportsMultithreading())
	//{

	//}
	//Multithread------------------------------------------------------------------------------

	LoadPreInitModules();
	//Memory------------------------------------------------------------------------------
	//FPlatformMisc::PlatformInit();
	//FPlatformMemory::Init();
	//Memory------------------------------------------------------------------------------

	//Physics-----------------------------------------------------------------------------
	//InitGamePhys();
	//Physics-----------------------------------------------------------------------------

	RHIInit(false);

	InitializeShaderTypes();

	//Setup Default material
	//UMaterialInterface::InitDefaultMaterials();
	//UMaterialInterface::AssertDefaultMaterialsExist();
	//UMaterialInterface::AssertDefaultMaterialsPostLoaded();

	RHIPostInit();

	//if Use ThreadRendering
	StartRenderingThread();

	//TODO:Other things

	if (GIsRequestingExit)
	{
		AppPreExit();
	}
	return 0;
}

int32 FEngineLoop::PreInit(const char* CmdLine)
{
	return 0;
}

void FEngineLoop::LoadPreInitModules()
{

}

void FEngineLoop::LoadCoreModules()
{

}

bool FEngineLoop::LoadStartupCoreModules()
{
	return false;
}

bool FEngineLoop::LoadStartupModules()
{
	return false;
}

int32 FEngineLoop::Init()
{
	//NewEngine
	GEngine = new UEngine();

	GEngine->Init(this);

	GEngine->StartHardwareSurvey();
	return 0;
}

void FEngineLoop::InitTime()
{

}

void FEngineLoop::Exit()
{
	if (GEngine != nullptr)
	{
		GEngine->PreExit();
	}
}

void FEngineLoop::Tick()
{

}

void FEngineLoop::PreInitHMDDevice()
{

}

bool FEngineLoop::AppInit()
{
	return true;
}

void FEngineLoop::AppPreExit()
{

}

void FEngineLoop::AppExit()
{

}