#include "D3D11RHIPCH.h"
#include "WindowsD3D11Device.h"
#include "RHI.h"
#include "WindowsD3D11DynamicRHI.h"

void SafeCreateDXGIFactory(IDXGIFactory1** DXGIFactory1)
{
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)DXGIFactory1);
}

FDynamicRHI* CreateRHI()
{
	FDynamicRHI* DynamicRHI = nullptr;

	IDXGIFactory1* DXGIFactory1;
	SafeCreateDXGIFactory(&DXGIFactory1);
	DynamicRHI = new FD3D11DynamicRHI();
	return DynamicRHI;
}

FD3D11DynamicRHI::FD3D11DynamicRHI()
{

}

FD3D11DynamicRHI::~FD3D11DynamicRHI()
{

}

void FD3D11DynamicRHI::Init()
{

}

void FD3D11DynamicRHI::PostInit()
{

}

void FD3D11DynamicRHI::Shutdown()
{

}