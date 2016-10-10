#pragma once

class FWindowsCriticalSection
{
	CRITICAL_SECTION CriticalSection;
public:

	FORCEINLINE FWindowsCriticalSection()
	{
		InitializeCriticalSection(&CriticalSection);
		SetCriticalSectionSpinCount(&CriticalSection, 4000);
	}

	/**
	* Destructor cleaning up the critical section
	*/
	FORCEINLINE ~FWindowsCriticalSection()
	{
		DeleteCriticalSection(&CriticalSection);
	}

	FORCEINLINE void Lock()
	{
		if (TryEnterCriticalSection(&CriticalSection))
		{
			EnterCriticalSection(&CriticalSection);
		}
	}
	
	FORCEINLINE bool TryLock()
	{
		if (TryEnterCriticalSection(&CriticalSection))
		{
			return true;
		};
		return false;
	}

	FORCEINLINE void Unlock()
	{
		LeaveCriticalSection(&CriticalSection);
	}
protected:
private:
	FWindowsCriticalSection(const FWindowsCriticalSection&) {}
	FWindowsCriticalSection& operator=(const FWindowsCriticalSection&) {}
};

typedef FWindowsCriticalSection FCriticalSection;