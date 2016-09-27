#pragma once

enum EThreadPriority
{
	TPri_Normal,
	TPri_AboveNormal,
	TPri_BelowNormal,
	TPri_Highest,
	TPri_Lowest,
	TPri_SlightlyBelowNormal,
};

class FRunnableThread
{
	static uint32 RunnableTlsSlot;
public:
	static uint32 GetTlsSlot();

	static FRunnableThread* Create(
	class FRunnable* InRunnable,
		const TCHAR* ThreadName,
		bool bAutoDeleteSelf,
		bool bAutoDeleteRunnable = false,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal,
		uint64 InThreadAffinityMask = 0);

	static FRunnableThread* Create(
	class FRunnable* InRunnable,
		const TCHAR* ThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal,
		uint64 InThreadAffinityMask = FPlatformAffinity::GetNoAffinityMask());

	virtual void SetThreadPriority(EThreadPriority NewPriority) = 0;

	virtual void Suspend(bool bShouldPause = true) = 0;

	virtual bool Kill(bool bShouldWait = true) = 0;

	virtual void WaitForCompletion() = 0;

	const uint32 GetThreadID() const
	{
		return ThreadID;
	}

	const std::string& GetThreadName() const
	{
		return ThreadName;
	}

	FRunnableThread();

	virtual ~FRunnableThread();
protected:

	virtual bool CreateInternal(FRunnable* InRunnable, const TCHAR* InThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal, uint64 InThreadAffinityMask = 0) = 0;

	void SetTls();

	void FreeTls();

	static FRunnableThread* GetRunnableThread()
	{
		FRunnableThread* RunnableThread = (FRunnableThread*)FPlatformTLS::GetTlsValue(RunnableTlsSlot);
		return RunnableThread;
	}

	std::string ThreadName;

	FRunnable* Runnable;

	FEvent* ThreadInitSyncEvent;

	uint64 ThreadAffinityMask;

	TArray<FTlsAutoCleanup*> TlsInstances;

	EThreadPriority ThreadPriority;

	uint32 ThreadID;
private:
	virtual void Tick() {}
};