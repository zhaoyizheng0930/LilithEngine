#pragma once
#include "../GenericPlatform/GenericWindow.h"
#include "WindowsApplication.h"

class FWindowsWindow :public FGenericWindow
{
public:
	static const TCHAR AppWindowClass[];
public:
	FWindowsWindow();

	HWND GetHWnd() const;

	virtual void Initialize(FWindowsApplication* application , FGenericWindow* InWindow, FGenericWindowDefinition* InDefinition, HINSTANCE InHInstance);

public:
	virtual void Minimize() override;
	virtual void Maximize() override;
	virtual void Restore() override;
	virtual void Show() override;
	virtual void Hide() override;
protected:
private:
	FGenericWindowDefinition* m_Definition;
	FWindowsApplication* m_Application;

	/** The window's handle. */
	HWND HWnd;

	bool bIsVisible : 1;
};