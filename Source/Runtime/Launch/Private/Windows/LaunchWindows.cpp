#include "../../Public/LaunchEngineLoop.h"

extern int32 GuardedMain(const TCHAR* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, int32 nCmdShow);

int32 WINAPI WinMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, TCHAR*, int32 nCmdShow)
{
	int32 ErrorLevel = 0;

	const TCHAR* CmdLine = ::GetCommandLine();

	ErrorLevel = GuardedMain(CmdLine, hInInstance, hPrevInstance, nCmdShow);

	return ErrorLevel;
}