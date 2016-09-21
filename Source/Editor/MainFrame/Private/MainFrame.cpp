#include "../Public/MainFrame.h"
#include "HAL/Platform.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindow.h"

SlateApplication* SlateApplicationIns = nullptr;

void CreateDefaultMainFrame(const bool bStartImmersive, const bool bStartPIE)
{

}

SlateApplication::SlateApplication()
{

}

void SlateApplication::MakeWindow()
{
	FGenericWindowDefinition Windowdefinition;
	Windowdefinition.WidthDesiredOnScreen = 1920.f;
	Windowdefinition.WidthDesiredOnScreen = 1080.f;
	Windowdefinition.Title = "LilithEngine";

	FGenericWindow* NewWindow = PlatformApplication->MakeWindow();

	PlatformApplication->InitializeWindow(NewWindow, &Windowdefinition);
}

void SlateApplication::AddWindow()
{
	SlateApplicationIns->MakeWindow();
}