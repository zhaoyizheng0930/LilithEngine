#pragma once



class FRenderResource
{
public:
	FRenderResource() :
		bInitialized(false)
	{

	}

	virtual void InitResource();

public:
	virtual void InitRHI() {}
	virtual void InitDynamicRHI() {}
	virtual void ReleaseDynamicRHI() {}

	virtual void ReleaseResource() {}
protected:
	bool bInitialized;
private:
};

extern void BeginInitResource(FRenderResource* Resource);