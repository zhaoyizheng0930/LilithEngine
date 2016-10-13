#include "RHIPCH.h"
#include "RHI.h"

FDynamicRHI* PlatformCreateDynamicRHI()
{
	FDynamicRHI* DynamicRHI = nullptr;

	DynamicRHI = CreateRHI();

	return DynamicRHI;
}