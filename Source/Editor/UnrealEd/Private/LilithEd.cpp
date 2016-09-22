#include "../Public/LilithEd.h"
#include "../../../Runtime/Engine/Public/UnrealEngine.h"
#include "../../../Editor/MainFrame/Public/MainFrame.h"
#include "GenericPlatform/GenericApplication.h"
#include "../../../Runtime/Core/Public/Windows/WindowsApplication.h"

#define IDICON_LilithEngine		123

int32 EditorInit(class IEngineLoop& EngineLoop)
{
	int32 ErrorLevel = EngineLoop.Init();

	//Maybe i need a splash screen,haha.

	// Startup Slate main frame and other editor windows
	// TODO_DLL:DirectlyCall,change to dll later
	CreateDefaultMainFrame(false, false);

	HICON AppIconHandle = LoadIcon(hInstance, MAKEINTRESOURCE(IDICON_LilithEngine));
	if (AppIconHandle == NULL)
	{
		AppIconHandle = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
	}

	CreateDefaultApplication(hInstance , AppIconHandle);

	SlateApplication::AddWindow();
	return 0;
}

void EditorExit()
{

}