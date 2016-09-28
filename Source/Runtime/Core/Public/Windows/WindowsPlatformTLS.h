#pragma once

class FWindowsPlatformTLS
{
public:
	static FORCEINLINE uint32 GetCurrentThreadId(void)
	{
		return ::GetCurrentThreadId();
	}
protected:
private:
};

typedef FWindowsPlatformTLS FPlatformTLS;