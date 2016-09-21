#pragma once
#include "../GenericPlatform/GenericWindow.h"

class FWindowsWindow :public FGenericWindow
{
public:
	FWindowsWindow();

	virtual void Initialize(FGenericWindow* InWindow, const FGenericWindowDefinition* InDefinition);
protected:
private:
};