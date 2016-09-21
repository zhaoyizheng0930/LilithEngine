#include "../../Public/Windows/WindowsApplication.h"
#include "../../Public/Windows/WindowsWindow.h"

GenericApplication* PlatformApplication = nullptr;

FWindowsApplication::FWindowsApplication()
{

}

FGenericWindow* FWindowsApplication::MakeWindow()
{
	FGenericWindow* newwindow = new FGenericWindow();
	return newwindow;
}

void FWindowsApplication::InitializeWindow(FGenericWindow* InWindow, const FGenericWindowDefinition* InDefinition)
{
	FWindowsWindow* Window = static_cast<FWindowsWindow*>(InWindow);

	Windows.push_back(Window);
	Window->Initialize(Window , InDefinition);
}
