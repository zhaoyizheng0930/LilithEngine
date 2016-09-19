#include <windows.h>

extern int GuardedMain(const TCHAR* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, int nCmdShow);

int WINAPI WinMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, int nCmdShow)
{
	int ErrorLevel = 0;

	const TCHAR* CmdLine = ::GetCommandLine();

	ErrorLevel = GuardedMain(CmdLine, hInInstance, hPrevInstance, nCmdShow);

	return ErrorLevel;
}