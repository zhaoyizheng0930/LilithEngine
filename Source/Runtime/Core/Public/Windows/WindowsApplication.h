#pragma once
#include "../GenericPlatform/GenericApplication.h"

class FWindowsApplication
	: public GenericApplication
{
public:
	FWindowsApplication();

	virtual FGenericWindow* MakeWindow();

	virtual void InitializeWindow(FGenericWindow* InWindow, const FGenericWindowDefinition* InDefinition);
protected:
private:
	std::vector<FGenericWindow*> Windows;
};