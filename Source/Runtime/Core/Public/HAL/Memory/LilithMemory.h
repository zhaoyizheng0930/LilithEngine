#pragma once
#include "MemoryBase.h"

struct FMemory
{
	static FORCEINLINE void* Memmove(void* Dest, const void* Src, SIZE_T Count)
	{
		return FPlatformMemory::Memmove(Dest, Src, Count);
	}

	static FORCEINLINE int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
	{
		return FPlatformMemory::Memcmp(Buf1, Buf2, Count);
	}

	static FORCEINLINE void* Memset(void* Dest, uint8 Char, SIZE_T Count)
	{
		return FPlatformMemory::Memset(Dest, Char, Count);
	}

	template< class T >
	static FORCEINLINE void Memset(T& Src, uint8 ValueToSet)
	{
		//static_assert(!TIsPointerType<T>::Value, "For pointers use the three parameters function");
		Memset(&Src, ValueToSet, sizeof(T));
	}

	static FORCEINLINE void* Memzero(void* Dest, SIZE_T Count)
	{
		return FPlatformMemory::Memzero(Dest, Count);
	}

	template< class T >
	static FORCEINLINE void Memzero(T& Src)
	{
		//static_assert(!TIsPointerType<T>::Value, "For pointers use the two parameters function");
		Memzero(&Src, sizeof(T));
	}

	static FORCEINLINE void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return FPlatformMemory::Memcpy(Dest, Src, Count);
	}

	template< class T >
	static FORCEINLINE void Memcpy(T& Dest, const T& Src)
	{
		//static_assert(!TIsPointerType<T>::Value, "For pointers use the three parameters function");
		Memcpy(&Dest, &Src, sizeof(T));
	}

	static FORCEINLINE void* BigBlockMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return FPlatformMemory::BigBlockMemcpy(Dest, Src, Count);
	}

	static FORCEINLINE void* StreamingMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return FPlatformMemory::StreamingMemcpy(Dest, Src, Count);
	}

	/*static FORCEINLINE void Memswap(void* Ptr1, void* Ptr2, SIZE_T Size)
	{
		FPlatformMemory::Memswap(Ptr1, Ptr2, Size);
	}*/

	template< class T >
	static FORCEINLINE void MemSet(T& Src, uint8 ValueToSet)
	{
		//static_assert(!TIsPointerType<T>::Value, "For pointers use the three parameters function");
		FMemory::Memset(&Src, ValueToSet, sizeof(T));
	}

	template< class T >
	static FORCEINLINE void MemZero(T& Src)
	{
		//static_assert(!TIsPointerType<T>::Value, "For pointers use the two parameters function");
		FMemory::Memset(&Src, 0, sizeof(T));
	}

	template< class T >
	static FORCEINLINE void MemCopy(T& Dest, const T& Src)
	{
		//static_assert(!TIsPointerType<T>::Value, "For pointers use the three parameters function");
		FMemory::Memcpy(&Dest, &Src, sizeof(T));
	}


	static FORCEINLINE void* SystemMalloc(SIZE_T Size)
	{
		return ::malloc(Size);
	}

	static FORCEINLINE void SystemFree(void* Ptr)
	{
		::free(Ptr);
	}


	// C style memory allocation stubs.
	static void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void Free(void* Original);
	static SIZE_T GetAllocSize(void* Original);
	static SIZE_T QuantizeSize(SIZE_T Count, uint32 Alignment);

	// Malloc for GPU mapped memory on UMA systems (XB1/PS4/etc)
	//static void* GPUMalloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	//static void* GPURealloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	//static void GPUFree(void* Original);
	private:
		static void GCreateMalloc();
		static void* MallocExternal(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
		static void* ReallocExternal(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
		static void FreeExternal(void* Original);
		static SIZE_T GetAllocSizeExternal(void* Original);
		static SIZE_T QuantizeSizeExternal(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
};