#pragma once
#include "../GenericPlatform/GenericApplication.h"

extern HINSTANCE hInstance;

extern void CreateDefaultApplication(const HINSTANCE InstanceHandle, const HICON IconHandle);

class FWindowsApplication
	: public GenericApplication
{
public:
	FWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle);

	virtual FGenericWindow* MakeWindow();

	virtual void InitializeWindow(FGenericWindow* InWindow, FGenericWindowDefinition* InDefinition);

public:
	//Misc
	virtual void PumpMessages() override;

	static int32 NumberOfCores();

	static int32 NumberOfCoresIncludingHyperthreads();

	static int32 NumberOfWorkerThreadsToSpawn();

protected:
	static LRESULT CALLBACK AppWndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);

	int32 ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);
private:
	static bool RegisterClass(const HINSTANCE HInstance, const HICON HIcon);
private:
	std::vector<FGenericWindow*> Windows;

	HINSTANCE InstanceHandle;
};

typedef FWindowsApplication FPlatformApplication;