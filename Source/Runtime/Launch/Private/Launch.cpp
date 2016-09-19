//support MultiPlatform later
#include <windows.h>
#include "Core.h"
#include "../Public/LaunchEngineLoop.h"

FEngineLoop	GEngineLoop;

int EnginePreInit(const TCHAR* CmdLine)
{
	int ErrorLevel = GEngineLoop.PreInit(CmdLine);

	return(ErrorLevel);
}

int EngineInit()
{
	int ErrorLevel = GEngineLoop.Init();

	return(ErrorLevel);
}

void EngineTick(void)
{
	GEngineLoop.Tick();
}

void EngineExit(void)
{
	// Make sure this is set
	GIsRequestingExit = true;

	GEngineLoop.Exit();
}

int GuardedMain(const TCHAR* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, int nCmdShow)
{
	int ErrorLevel = EnginePreInit(CmdLine);

	// exit if PreInit failed.
	if (ErrorLevel != 0 || GIsRequestingExit)
	{
		return ErrorLevel;
	}

	//TODO_SLOWLOADING
	//FScopedSlowTask SlowTask(100, NSLOCTEXT("EngineInit", "EngineInit_Loading", "Loading..."));
	//SlowTask.EnterProgressFrame(80);
	//SlowTask.EnterProgressFrame(20);
	
	//TODO_EditorMode
	//ErrorLevel = EditorInit(GEngineLoop);

	ErrorLevel = EngineInit();

	while (!GIsRequestingExit)
	{
		EngineTick();
	}

	//TODO_EditorMode
	//EditorExit();

	return ErrorLevel;
}