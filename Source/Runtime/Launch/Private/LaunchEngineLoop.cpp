#include "../Public/LaunchEngineLoop.h"

FEngineLoop::FEngineLoop()
{

}

int FEngineLoop::PreInit(int ArgC, char* ArgV[], const char* AdditionalCommandline)
{
	return 0;
}

int FEngineLoop::PreInit(const char* CmdLine)
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

int FEngineLoop::Init()
{
	return 0;
}

void FEngineLoop::InitTime()
{

}

void FEngineLoop::Exit()
{

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