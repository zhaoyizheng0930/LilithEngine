#include "D3D11RHIPCH.h"
#include "D3D11ConstantBuffer.h"

const uint32 GConstantBufferSizes[MAX_CONSTANT_BUFFER_SLOTS] =
{
	// CBs must be a multiple of 16
	(uint32)Align(MAX_GLOBAL_CONSTANT_BUFFER_SIZE, 16),
};