#pragma once
#include "GenericWindowDefinition.h"

class FGenericWindow
{
public:
	FGenericWindow();

	virtual void Initialize(FGenericWindow* InWindow, const FGenericWindowDefinition* InDefinition);

public:
	virtual void Minimize();
	virtual void Maximize();
	virtual void Restore();
	virtual void Show();
	virtual void Hide();
protected:
private:
};