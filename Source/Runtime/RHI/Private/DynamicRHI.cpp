#include "RHIPCH.h"
#include "../Public/RHI.h"
#include "../Public/DynamicRHI.h"

FDynamicRHI* GDynamicRHI = nullptr;
std::wstring GRHIAdapterName = L"";
uint32 GRHIVendorId = 0;
uint32 GRHIDeviceId = 0;

void RHIInit(bool bHasEditorToken)
{
	if (GDynamicRHI == nullptr)
	{
		GDynamicRHI = PlatformCreateDynamicRHI();
		if (GDynamicRHI)
		{
			GDynamicRHI->Init();
			//GRHICommandList
		}
	}
}

void RHIPostInit()
{
	if (GDynamicRHI)
	{
		GDynamicRHI->PostInit();
	}
}

void RHIExit()
{
	if (GDynamicRHI)
	{
		GDynamicRHI->Shutdown();
		delete GDynamicRHI;
		GDynamicRHI = nullptr;
	}
}