#include "../Public/LaunchEngineLoop.h"
#include "HAL/PlatformIncludes.h"
#include "HAL/PlatformTLS.h"
#include "RHI.h"
#include "ShaderCore.h"
#include "RenderingThread.h"
#include "EngineGlobals.h"
#include "HAL/ThreadingBase.h"
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
	if (FPlatformProcess::SupportsMultithreading())
	{
		GThreadPool = FQueuedThreadPool::Allocate();
		int NumThreadsInThreadPool = FPlatformApplication::NumberOfWorkerThreadsToSpawn();

		GThreadPool->Create(NumThreadsInThreadPool);

		//Only in editor. if you need to build something in editor,Open it.
		//GLargeThreadPool = FQueuedThreadPool::Allocate();		//UseFor Build lighting in editor run.
		//int32 NumThreadsInLargeThreadPool = FMath::Max(FPlatformMisc::NumberOfCoresIncludingHyperthreads() - 2, 2);
		//GLargeThreadPool->Create(NumThreadsInLargeThreadPool)
	}
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
	// Initialize the RHI.
	RHIInit(true);

	RHIPostInit();

	if (GUseThreadedRendering)
	{

	}
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
	// Set FApp::CurrentTime, FApp::DeltaTime and potentially wait to enforce max tick rate.
	//GEngine->UpdateTimeAndHandleMaxTickRate();
}

void FEngineLoop::PreInitHMDDevice()
{

}

bool FEngineLoop::AppInit()
{
	//PreInitHMDDevice();
	return true;
}

void FEngineLoop::AppPreExit()
{

}

void FEngineLoop::AppExit()
{

}