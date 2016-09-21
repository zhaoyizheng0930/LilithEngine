#include "../Public/LilithEd.h"
#include "../../../Runtime/Engine/Public/UnrealEngine.h"
#include "../../../Editor/MainFrame/Public/MainFrame.h"

int32 EditorInit(class IEngineLoop& EngineLoop)
{
	int32 ErrorLevel = EngineLoop.Init();

	//Maybe i need a splash screen,haha.

	// Startup Slate main frame and other editor windows
	// TODO_DLL:DirectlyCall,change to dll later
	CreateDefaultMainFrame(false, false);

	return 0;
}

void EditorExit()
{

}