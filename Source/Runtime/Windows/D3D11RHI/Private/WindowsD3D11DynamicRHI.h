#pragma once
#include "RHI.h"
#include "D3D11RHI.h"

class FD3D11DynamicRHI:public FDynamicRHI
{
public:
	FD3D11DynamicRHI();

	~FD3D11DynamicRHI();

	virtual void Init();

	virtual void PostInit();

	virtual void Shutdown();
protected:
private:
};