#pragma once

#include "../GenericPlatform/GenericPlatform.h"
#include <windows.h>

struct FWindowsPlatformTypes : public FGenericPlatformTypes
{
#ifdef _WIN64
	typedef unsigned __int64	SIZE_T;
	typedef __int64				SSIZE_T;
#else
	typedef unsigned long		SIZE_T;
	typedef long				SSIZE_T;
#endif
};

typedef FWindowsPlatformTypes FPlatformTypes;


#define VARARGS     __cdecl														/* Functions with variable arguments */
#define CDECL	    __cdecl														/* Standard C function */
#define STDCALL		__stdcall													/* Standard calling convention */
#define FORCEINLINE __forceinline												/* Force code to be inline */
#define FORCENOINLINE __declspec(noinline)										/* Force code to NOT be inline */
//#define FUNCTION_CHECK_RETURN(...) __declspec("SAL_checkReturn") __VA_ARGS__	/* Wrap a function signature in this to warn that callers should not ignore the return value. */



// DLL export and import definitions
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)