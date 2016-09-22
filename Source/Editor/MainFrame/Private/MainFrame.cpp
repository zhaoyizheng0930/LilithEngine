#include "../Public/MainFrame.h"
#include "HAL/Platform.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindow.h"

SlateApplication* SlateApplicationIns = nullptr;

void CreateDefaultMainFrame(const bool bStartImmersive, const bool bStartPIE)
{
	SlateApplicationIns = new SlateApplication();
}

SlateApplication::SlateApplication()
{

}

void SlateApplication::MakeWindow()
{
	//TODO_Config:
	FGenericWindowDefinition Windowdefinition;
	Windowdefinition.WidthDesiredOnScreen = 1280;
	Windowdefinition.HeightDesiredOnScreen = 720;
	Windowdefinition.XDesiredPositionOnScreen = 100;
	Windowdefinition.YDesiredPositionOnScreen = 100;
	Windowdefinition.Title = "LilithEngine";

	FGenericWindow* NewWindow = PlatformApplication->MakeWindow();

	PlatformApplication->InitializeWindow(NewWindow, &Windowdefinition);
}

void SlateApplication::AddWindow()
{
	SlateApplicationIns->MakeWindow();
}