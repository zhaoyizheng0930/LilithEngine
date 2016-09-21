//support MultiPlatform later
#include "../Public/LaunchEngineLoop.h"
#include "LilithEd.h"

FEngineLoop	GEngineLoop;

int32 EnginePreInit(const TCHAR* CmdLine)
{
	int32 ErrorLevel = GEngineLoop.PreInit(CmdLine);

	return(ErrorLevel);
}

int32 EngineInit()
{
	int32 ErrorLevel = GEngineLoop.Init();

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

int32 GuardedMain(const TCHAR* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, int32 nCmdShow)
{
	int32 ErrorLevel = EnginePreInit(CmdLine);

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
	ErrorLevel = EditorInit(GEngineLoop);

	ErrorLevel = EngineInit();

	while (!GIsRequestingExit)
	{
		EngineTick();
	}

	//TODO_EditorMode
	//EditorExit();

	return ErrorLevel;
}