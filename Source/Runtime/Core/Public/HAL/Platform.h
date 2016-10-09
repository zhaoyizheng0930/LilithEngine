#pragma once

#ifdef PLATFORM_WINDOWS
	#include "../Windows/WindowsPlatform.h"
#endif

// Unsigned base types.
typedef FPlatformTypes::uint8		uint8;		///< An 8-bit unsigned integer.
typedef FPlatformTypes::uint16		uint16;		///< A 16-bit unsigned integer.
typedef FPlatformTypes::uint32		uint32;		///< A 32-bit unsigned integer.
typedef FPlatformTypes::uint64		uint64;		///< A 64-bit unsigned integer.

												// Signed base types.
typedef	FPlatformTypes::int8		int8;		///< An 8-bit signed integer.
typedef FPlatformTypes::int16		int16;		///< A 16-bit signed integer.
typedef FPlatformTypes::int32		int32;		///< A 32-bit signed integer.
typedef FPlatformTypes::int64		int64;		///< A 64-bit signed integer.

												// Character types.
typedef FPlatformTypes::ANSICHAR	ANSICHAR;	///< An ANSI character. Normally a signed type.
typedef FPlatformTypes::WIDECHAR	WIDECHAR;	///< A wide character. Normally a signed type.
typedef FPlatformTypes::TCHAR		TCHAR;		///< Either ANSICHAR or WIDECHAR, depending on whether the platform supports wide characters or the requirements of the licensee.
typedef FPlatformTypes::CHAR8		UTF8CHAR;	///< An 8-bit character containing a UTF8 (Unicode, 8-bit, variable-width) code unit.
typedef FPlatformTypes::CHAR16		UCS2CHAR;	///< A 16-bit character containing a UCS2 (Unicode, 16-bit, fixed-width) code unit, used for compatibility with 'Windows TCHAR' across multiple platforms.
typedef FPlatformTypes::CHAR16		UTF16CHAR;	///< A 16-bit character containing a UTF16 (Unicode, 16-bit, variable-width) code unit.
typedef FPlatformTypes::CHAR32		UTF32CHAR;	///< A 32-bit character containing a UTF32 (Unicode, 32-bit, fixed-width) code unit.

//typedef FPlatformTypes::UPTRINT UPTRINT;		///< An unsigned integer the same size as a pointer
//typedef FPlatformTypes::PTRINT PTRINT;			///< A signed integer the same size as a pointer
typedef FPlatformTypes::SIZE_T SIZE_T;			///< An unsigned integer the same size as a pointer, the same as UPTRINT
typedef FPlatformTypes::SSIZE_T SSIZE_T;		///< An integer the same size as a pointer, the same as PTRINT

typedef FPlatformTypes::TYPE_OF_NULL	TYPE_OF_NULL;		///< The type of the NULL constant.
typedef FPlatformTypes::TYPE_OF_NULLPTR	TYPE_OF_NULLPTR;	///< The type of the C++ nullptr keyword.