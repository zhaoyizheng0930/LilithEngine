#pragma once

class FDynamicRHI
{
public:
	virtual ~FDynamicRHI() {}

	virtual void Init() = 0;

	virtual void PostInit() {}

	virtual void Shutdown() = 0;

	/////// RHI Methods


protected:
private:
};

extern FDynamicRHI* GDynamicRHI;

FDynamicRHI* PlatformCreateDynamicRHI();