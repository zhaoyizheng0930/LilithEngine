#pragma once

#define MaxSimultaneousRenderTargets 8

enum EResourceLockMode
{
	RLM_ReadOnly,
	RLM_WriteOnly,
	RLM_Num
};


/** The base type of a value in a uniform buffer. */
enum EUniformBufferBaseType
{
	UBMT_INVALID,
	UBMT_BOOL,
	UBMT_INT32,
	UBMT_UINT32,
	UBMT_FLOAT32,
	UBMT_STRUCT,
	UBMT_SRV,
	UBMT_UAV,
	UBMT_SAMPLER,
	UBMT_TEXTURE
};

enum EUniformBufferUsage
{
	// the uniform buffer is temporary, used for a single draw call then discarded
	UniformBuffer_SingleDraw = 0,
	// the uniform buffer is used for multiple draw calls but only for the current frame
	UniformBuffer_SingleFrame,
	// the uniform buffer is used for multiple draw calls, possibly across multiple frames
	UniformBuffer_MultiFrame,
};

enum EBufferUsageFlags
{
	// Mutually exclusive write-frequency flags
	BUF_Static = 0x0001, // The buffer will be written to once.
	BUF_Dynamic = 0x0002, // The buffer will be written to occasionally, GPU read only, CPU write only.  The data lifetime is until the next update, or the buffer is destroyed.
	BUF_Volatile = 0x0004, // The buffer's data will have a lifetime of one frame.  It MUST be written to each frame, or a new one created each frame.

						   // Mutually exclusive bind flags.
	BUF_UnorderedAccess = 0x0008, // Allows an unordered access view to be created for the buffer.

								  /** Create a byte address buffer, which is basically a structured buffer with a uint32 type. */
	BUF_ByteAddressBuffer = 0x0020,
	/** Create a structured buffer with an atomic UAV counter. */
	BUF_UAVCounter = 0x0040,
	/** Create a buffer that can be bound as a stream output target. */
	BUF_StreamOutput = 0x0080,
	/** Create a buffer which contains the arguments used by DispatchIndirect or DrawIndirect. */
	BUF_DrawIndirect = 0x0100,
	/**
	* Create a buffer that can be bound as a shader resource.
	* This is only needed for buffer types which wouldn't ordinarily be used as a shader resource, like a vertex buffer.
	*/
	BUF_ShaderResource = 0x0200,

	/**
	* Request that this buffer is directly CPU accessible
	* (@todo josh: this is probably temporary and will go away in a few months)
	*/
	BUF_KeepCPUAccessible = 0x0400,

	/**
	* Provide information that this buffer will contain only one vertex, which should be delivered to every primitive drawn.
	* This is necessary for OpenGL implementations, which need to handle this case very differently (and can't handle GL_HALF_FLOAT in such vertices at all).
	*/
	BUF_ZeroStride = 0x0800,

	/** Buffer should go in fast vram (hint only) */
	BUF_FastVRAM = 0x1000,

	// Helper bit-masks
	BUF_AnyDynamic = (BUF_Dynamic | BUF_Volatile),
};

enum ETextureReallocationStatus
{
	TexRealloc_Succeeded = 0,
	TexRealloc_Failed,
	TexRealloc_InProgress,
};

enum ERenderQueryType
{
	// e.g. WaitForFrameEventCompletion()
	RQT_Undefined,
	// Result is the number of samples that are not culled (divide by MSAACount to get pixels)
	RQT_Occlusion,
	// Result is time in micro seconds = 1/1000 ms = 1/1000000 sec
	RQT_AbsoluteTime,
};