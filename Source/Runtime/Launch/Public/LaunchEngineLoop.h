#pragma once
#include "Core.h"


class FEngineLoop
{
public:
	FEngineLoop();

	int32 PreInit(int32 ArgC, char* ArgV[], const char* AdditionalCommandline = nullptr);

	int32 PreInit(const char* CmdLine);

	void LoadPreInitModules();

	void LoadCoreModules();

	bool LoadStartupCoreModules();

	bool LoadStartupModules();

	virtual int32 Init();// override;

	void InitTime();

	void Exit();

	virtual void Tick();// override;

	static void PreInitHMDDevice();

public:
	static bool AppInit();

	static void AppPreExit();

	static void AppExit();
protected:
private:
};