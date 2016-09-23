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
protected:
private:
};

//TODO:Move it to slate application later.
extern GenericApplication* PlatformApplication;