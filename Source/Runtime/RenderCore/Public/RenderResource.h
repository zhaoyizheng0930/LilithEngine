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

template<uint32 Size,bool TThreadSafe = true>
class TBoundShaderStateHistory:public FRenderResource
{
public:
	TBoundShaderStateHistory():
		NextBoundShaderStateIndex(0)
	{}

	void Add(FRHIBoundShaderState* BoundShaderState)
	{
		if (TThreadSafe)
		{
			BoundShaderStateHistoryLock.Lock();
		}
		BoundShaderStateHistoryLock[NextBoundShaderStateIndex] = BoundShaderState;
		NextBoundShaderStateIndex = (NextBoundShaderStateIndex + 1) % Size;
		if (TThreadSafe)
		{
			BoundShaderStateHistoryLock.Unlock();
		}
	}

	FRHIBoundShaderState* GetLast()
	{
		uint32 LastIndex = NextBoundShaderStateIndex == 0? Size - 1 : NextBoundShaderStateIndex - 1;
		return BoundShaderStates[LastIndex];
	}

	void Release()
	{
		if (TThreadSafe)
		{
			BoundShaderStateHistoryLock.Lock();
		}

		for (size_t Index = 0; Index < Size; Index++)
		{
			delete BoundShaderStates[Index];
			BoundShaderStates[Index] = nullptr;
		}

		if (TThreadSafe)
		{
			BoundShaderStateHistoryLock.Unlock();
		}
	}

private:
	FRHIBoundShaderState* BoundShaderStates[Size];
	uint32 NextBoundShaderStateIndex;
	FCriticalSection BoundShaderStateHistoryLock;
};


extern void BeginInitResource(FRenderResource* Resource);