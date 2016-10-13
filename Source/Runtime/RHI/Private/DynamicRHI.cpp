#include "RHIPCH.h"
#include "../Public/RHI.h"
#include "../Public/DynamicRHI.h"

FDynamicRHI* GDynamicRHI = nullptr;

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