#pragma once
#include "Core.h"

extern void InitializeShaderTypes();

extern void UninitializeShaderTypes();

struct FBaseShaderResourceTable
{
	//Bit indicating whitch resource tables contain resources bound to this shader
	uint32 ResourceTableBits;
};