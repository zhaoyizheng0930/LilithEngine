#pragma once



class FRenderResource
{
public:
	virtual void InitDynamicRHI() {}
	virtual void ReleaseDynamicRHI() {}

	virtual void ReleaseResource() {}
protected:
private:
};

extern void BeginInitResource(FRenderResource* Resource);