#include "RenderCorePCH.h"
#include "RenderResource.h"
#include "RHI.h"

void BeginInitResource(FRenderResource* Resource)
{
	//ZYZ_TODO:Generate RenderCommand;
}

void FRenderResource::InitResource()
{
	if (!bInitialized)
	{
		//ZYZ_TODO:Resource list Link
		if (GIsRHIInitialized)
		{
			InitDynamicRHI();
			InitRHI();
		}
		bInitialized = true;
	}
}