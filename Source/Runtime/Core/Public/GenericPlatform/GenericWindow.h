#pragma once
#include "../Core.h"
#include "GenericWindowDefinition.h"

class FGenericWindow
{
public:
	FGenericWindow();

	virtual void Initialize(FGenericWindow* InWindow, const FGenericWindowDefinition* InDefinition);
protected:
private:
};