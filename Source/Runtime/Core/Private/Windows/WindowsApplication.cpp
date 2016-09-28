#include "../../Public/Windows/WindowsApplication.h"
#include "../../Public/Windows/WindowsWindow.h"
#include "../../Public/HAL/LilithMemory.h"

HINSTANCE hInstance = NULL;

//FWindowsApplication* WindowsApplication = nullptr;

void CreateDefaultApplication(const HINSTANCE InstanceHandle, const HICON IconHandle)
{
	PlatformApplication = new FWindowsApplication(InstanceHandle , IconHandle);
}

FWindowsApplication::FWindowsApplication(const HINSTANCE HInstance , const HICON IconHandle):
	InstanceHandle(HInstance)
{
	//::DisableProcessWindowsGhosting();
	const bool bClassRegistered = RegisterClass(InstanceHandle, IconHandle);

}

FGenericWindow* FWindowsApplication::MakeWindow()
{
	FGenericWindow* newwindow = new FWindowsWindow();
	return newwindow;
}

void FWindowsApplication::InitializeWindow(FGenericWindow* InWindow, FGenericWindowDefinition* InDefinition)
{
	FWindowsWindow* Window = static_cast<FWindowsWindow*>(InWindow);

	Windows.push_back(Window);
	Window->Initialize(this , Window , InDefinition, InstanceHandle);
	Window->Show();
}

void FWindowsApplication::PumpMessages()
{
	MSG Message;
	while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}

int32 FWindowsApplication::NumberOfCores()
{
	static int32 CoreCount = 0;
	if (GbUseHyperthreads)
	{
		CoreCount = NumberOfCoresIncludingHyperthreads();
	}
	else
	{
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION InfoBuffer = NULL;
		::DWORD BufferSize = 0;

		// Get the size of the buffer to hold processor information.
		::BOOL Result = GetLogicalProcessorInformation(InfoBuffer, &BufferSize);

		// Allocate the buffer to hold the processor info.
		InfoBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)FMemory::Malloc(BufferSize);

		// Get the actual information.
		Result = GetLogicalProcessorInformation(InfoBuffer, &BufferSize);

		// Count physical cores
		const int32 InfoCount = (int32)(BufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
		for (int32 Index = 0; Index < InfoCount; ++Index)
		{
			SYSTEM_LOGICAL_PROCESSOR_INFORMATION* Info = &InfoBuffer[Index];
			if (Info->Relationship == RelationProcessorCore)
			{
				CoreCount++;
			}
		}
		FMemory::Free(InfoBuffer);
	}

	return CoreCount;
}

int32 FWindowsApplication::NumberOfCoresIncludingHyperthreads()
{
	static int32 CoreCount = 0;
	if (CoreCount == 0)
	{
		// Get the number of logical processors, including hyperthreaded ones.
		SYSTEM_INFO SI;
		GetSystemInfo(&SI);
		CoreCount = (int32)SI.dwNumberOfProcessors;
	}
	return CoreCount;
}

LRESULT CALLBACK FWindowsApplication::AppWndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	FWindowsApplication* windowsapp = static_cast<FWindowsApplication*>(PlatformApplication);
	return windowsapp->ProcessMessage(hwnd, msg, wParam, lParam);
}

int32 FWindowsApplication::ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	//Input
	switch (msg)
	{
	// Mouse Movement
	case WM_INPUT:
	{

		break;
	}
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool FWindowsApplication::RegisterClass(const HINSTANCE HInstance, const HICON HIcon)
{
	WNDCLASS wc;
	
	FMemory::Memzero(&wc, sizeof(wc));
	wc.style = CS_DBLCLKS; // We want to receive double clicks
	wc.lpfnWndProc = AppWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HInstance;
	wc.hIcon = HIcon;
	wc.hCursor = NULL;	// We manage the cursor ourselves
	wc.hbrBackground = NULL; // Transparent
	wc.lpszMenuName = NULL;
	wc.lpszClassName = FWindowsWindow::AppWindowClass;

	if (!::RegisterClass(&wc))
	{
		//ShowLastError();

		// @todo Slate: Error message should be localized!
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);

		return false;
	}

	return true;
}
