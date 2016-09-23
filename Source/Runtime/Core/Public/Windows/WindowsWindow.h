#pragma once
#include "../GenericPlatform/GenericWindow.h"
#include "WindowsApplication.h"
#include "../HAL/Platform.h"

class FWindowsWindow :public FGenericWindow
{
public:
	static const TCHAR AppWindowClass[];
public:
	FWindowsWindow();

	HWND GetHWnd() const;

	virtual void Initialize(FWindowsApplication* application , FGenericWindow* InWindow, FGenericWindowDefinition* InDefinition, HINSTANCE InHInstance);
protected:
private:
	FGenericWindowDefinition* m_Definition;
	FWindowsApplication* m_Application;

	/** The window's handle. */
	HWND HWnd;
};