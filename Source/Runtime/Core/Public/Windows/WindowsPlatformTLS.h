#pragma once

class FWindowsPlatformTLS
{
	static FORCEINLINE uint32 GetCurrentThreadId(void)
	{
		return ::GetCurrentThreadId();
	}

public:
protected:
private:
};

typedef FWindowsPlatformTLS FPlatformTLS;