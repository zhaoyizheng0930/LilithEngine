#include "../../Public/Windows/WindowsApplication.h"
#include "../../Public/Windows/WindowsWindow.h"

void CreateDefaultApplication()
{
	PlatformApplication = new FWindowsApplication();
}

FWindowsApplication::FWindowsApplication()
{

}

FGenericWindow* FWindowsApplication::MakeWindow()
{
	FGenericWindow* newwindow = new FWindowsWindow();
	return newwindow;
}

void FWindowsApplication::InitializeWindow(FGenericWindow* InWindow, const FGenericWindowDefinition* InDefinition)
{
	FWindowsWindow* Window = static_cast<FWindowsWindow*>(InWindow);

	Windows.push_back(Window);
	Window->Initialize(Window , InDefinition);
}
