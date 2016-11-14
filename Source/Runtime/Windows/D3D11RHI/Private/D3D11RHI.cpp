#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "D3D11RHI.h"


int32 GNumDrawCallsRHI = 0;
int32 GNumPrimitivesDrawnRHI = 0;

int32 GNumActiveGPUsForRendering = 1;
bool bGFinishCurrentFrame = true;

extern void UniformBufferBeginFrame();

void FD3D11DynamicRHI::RHIBeginFram()
{
	//Init DrawCallNum
	GNumDrawCallsRHI = 0;
	GNumPrimitivesDrawnRHI = 0;

	//Uniform Buffer Update
	UniformBufferBeginFrame();
}

void FD3D11DynamicRHI::RHIEndFram()
{
	CurrentComputeShader = nullptr;//Why
}

void FD3D11DynamicRHI::RHIBeginScene()
{

}

void FD3D11DynamicRHI::RHIEndScene()
{

}