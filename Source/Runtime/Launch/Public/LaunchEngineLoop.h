#pragma once


class FEngineLoop
{
public:
	FEngineLoop();

	int PreInit(int ArgC, char* ArgV[], const char* AdditionalCommandline = nullptr);

	int PreInit(const char* CmdLine);

	void LoadPreInitModules();

	void LoadCoreModules();

	bool LoadStartupCoreModules();

	bool LoadStartupModules();

	virtual int Init();// override;

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