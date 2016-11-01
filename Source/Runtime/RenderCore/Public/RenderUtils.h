#pragma once
#include "Core.h"
#include "PixelFormat.h"

/** Information about a pixel format. */
struct FPixelFormatInfo
{
	const char*	Name;
	int32				BlockSizeX,
		BlockSizeY,
		BlockSizeZ,
		BlockBytes,
		NumComponents;
	/** Platform specific token, e.g. D3DFORMAT with D3DDrv										*/
	uint32			PlatformFormat;
	/** Whether the texture format is supported on the current platform/ rendering combination	*/
	bool			Supported;
	EPixelFormat	UnrealFormat;
};

extern FPixelFormatInfo GPixelFormats[PF_MAX];