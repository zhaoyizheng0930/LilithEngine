#pragma once
#include "../HAL/Platform.h"

class FGenericPlatformMemory
{
public:
	static FORCEINLINE void* Memmove(void* Dest, const void* Src, SIZE_T Count)
	{
		return memmove(Dest, Src, Count);
	}

	static FORCEINLINE int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
	{
		return memcmp(Buf1, Buf2, Count);
	}

	static FORCEINLINE void* Memset(void* Dest, uint8 Char, SIZE_T Count)
	{
		return memset(Dest, Char, Count);
	}

	static FORCEINLINE void* Memzero(void* Dest, SIZE_T Count)
	{
		return memset(Dest, 0, Count);
	}

	static FORCEINLINE void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	/** Memcpy optimized for big blocks. */
	static FORCEINLINE void* BigBlockMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	/** On some platforms memcpy optimized for big blocks that avoid L2 cache pollution are available */
	static FORCEINLINE void* StreamingMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}
protected:
private:
};