#pragma once
#include "GenericWindow.h"
#include "GenericWindowDefinition.h"

extern void CreateDefaultApplication(const HINSTANCE InstanceHandle, const HICON IconHandle);

class GenericApplication
{
public:
	GenericApplication() {}

	virtual FGenericWindow* MakeWindow() { return NULL; }

	virtual void InitializeWindow(FGenericWindow* Window, FGenericWindowDefinition* InDefinition) { }

public:
	//Misc
	virtual void PumpMessages() { }

	static int32 NumberOfCores() { return 1; }

	static int32 NumberOfCoresIncludingHyperthreads() { return GenericApplication::NumberOfCores(); }
protected:
private:
};

//TODO:Move it to slate application later.
extern GenericApplication* PlatformApplication;