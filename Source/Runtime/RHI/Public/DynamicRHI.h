#pragma once

class FDynamicRHI
{
public:
	virtual ~FDynamicRHI() {}

	virtual void Init() = 0;

	virtual void PostInit() {}

	virtual void Shutdown() = 0;

public:
	/////// RHI Methods
	virtual void RHISetScissorRect(bool bEnable, uint32 MinX, uint32 MinY, uint32 MaxX, uint32 MaxY) = 0;

protected:
private:
};

extern FDynamicRHI* GDynamicRHI;

FDynamicRHI* PlatformCreateDynamicRHI();