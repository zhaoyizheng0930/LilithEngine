#include "../../Public/Windows/WindowsWindow.h"

const TCHAR FWindowsWindow::AppWindowClass[] = TEXT("LilithWindow");

FWindowsWindow::FWindowsWindow():
	m_Definition(NULL),
	m_Application(NULL)
{

}

HWND FWindowsWindow::GetHWnd() const
{
	return HWnd;
}

void FWindowsWindow::Initialize(FWindowsApplication* application , FGenericWindow* InWindow, FGenericWindowDefinition* InDefinition, HINSTANCE InHInstance)
{
	m_Definition = InDefinition;
	m_Application = application;

	uint32 WindowExStyle = WS_EX_APPWINDOW;
	uint32 WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;

	//WindowStyle |= WS_POPUP | WS_BORDER;

	int32 WindowX = InDefinition->XDesiredPositionOnScreen;
	int32 WindowY = InDefinition->YDesiredPositionOnScreen;

	int32 WindowWidth = InDefinition->WidthDesiredOnScreen;
	int32 WindowHeight = InDefinition->HeightDesiredOnScreen;

	//WindowsBorder
	//RECT BorderRect = { 0, 0, 0, 0 };
	//::AdjustWindowRectEx(&BorderRect, WindowStyle, false, WindowExStyle);

	HWnd = CreateWindowEx(
		WindowExStyle,
		AppWindowClass,
		InDefinition->Title.c_str(),
		WindowStyle,
		WindowX, WindowY,
		WindowWidth, WindowHeight,
		NULL,
		NULL, InHInstance, NULL);


}