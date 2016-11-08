#pragma once



class FRenderResource
{
public:
	virtual void InitDynamicRHI() {}
	virtual void ReleaseDynamicRHI() {}
protected:
private:
};

extern void BeginInitResource(FRenderResource* Resource);