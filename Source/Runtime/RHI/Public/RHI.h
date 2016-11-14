#pragma once
#include "HAL/PlatformIncludes.h"
#include "RHIDefinitions.h"

class FDynamicRHI;

/** Initializes the RHI. */
extern void RHIInit(bool bHasEditorToken);

/** Performs additional RHI initialization before the render thread starts. */
extern void RHIPostInit();

/** Shuts down the RHI. */
extern void RHIExit();

extern FDynamicRHI* CreateRHI();

extern std::wstring GRHIAdapterName;
extern uint32 GRHIVendorId;
extern uint32 GRHIDeviceId;


extern int32 GNumActiveGPUsForRendering;

extern bool bGFinishCurrentFrame;


extern int32 GNumDrawCallsRHI;
extern int32 GNumPrimitivesDrawnRHI;

enum ESamplerFilter
{
	SF_Point,				//点采样:取最接近的Mipmap上的点
	SF_Bilinear,			//双线性采样:取最接近Level的mipmap上的4个点做双线性插值
	SF_Trilinear,			//三线性采样:取min mag上面每个四个点做双线性插值，得到的两个点在做线性插值
	SF_AnisotropicPoint,	//各向异性采样:取非正方形的四边形上的四个点做双线性差值，可以解决视角与平面偏差问题
	SF_AnisotropicLinear,
};
//
//Mip: mipmap 多级描述 每级是上一级的1/2 * 1/2
//Min: 
//Mag: 

enum ESamplerAddressMode	//当UV超过1时UV的计算方式
{
	AM_Wrap,
	AM_Clamp,
	AM_Mirror,
	AM_Border
};

enum ESamplerCompareFunction
{
	SCF_Never,
	SCF_Less
};

struct FSamplerStateInitializerRHI
{
public:
	FSamplerStateInitializerRHI() {}
	FSamplerStateInitializerRHI(
		ESamplerFilter InFilter,
		ESamplerAddressMode InAddressU = AM_Wrap,
		ESamplerAddressMode InAddressV = AM_Wrap,
		ESamplerAddressMode InAddressW = AM_Wrap,
		int32 InMipBias = 0,
		float InMinMipLevel = 0,
		float InMaxMipLevel = FLT_MAX,
		int32 InMaxAnisotropy = 0,
		uint32 InBorderColor = 0,
		ESamplerCompareFunction InSamplerComparisonFunction = SCF_Never
		):
			Filter(InFilter),
			AddressU(InAddressU),
			AddressV(InAddressV),
			AddressW(InAddressW),
			MipBias(InMipBias),
			MinMipLevel(InMinMipLevel),
			MaxMipLevel(InMaxMipLevel),
			MaxAnisotropy(InMaxAnisotropy),
			BorderColor(InBorderColor),
			SamplerComparisonFunction(InSamplerComparisonFunction)
	{}

	ESamplerFilter Filter; //????
	ESamplerAddressMode AddressU;	//??0,1?????
	ESamplerAddressMode AddressV;
	ESamplerAddressMode AddressW;
	int32 MipBias;	//mipmap level ????
	float MinMipLevel;	//Min LOD
	float MaxMipLevel;	//Max LOD
	int32 MaxAnisotropy;	//?????????????????
	uint32 BorderColor;	//AdressMode?? Bordor??????
	ESamplerCompareFunction SamplerComparisonFunction;//????
};

enum ERasterizerFillMode //填充模式
{
	FM_Point,			//点填充
	FM_Wireframe,		//线框填充
	FM_Solid			//实心填充
};

enum ERasterizerCullMode	//光栅化剔除模式，指背面剔除
{
	CM_None,		
	CM_CW,					//背面剔除
	CM_CCW					//正面剔除
};


struct FRasterizerStateInitializerRHI
{
	ERasterizerFillMode FillMode;
	ERasterizerCullMode CullMode;
	float DepthBias;
	float SlopeScaleDepthBias;
	bool bAllowMSAA;
	bool bEnableLineAA;
};

enum ECompareFunction
{
	CF_Less,
	CF_LessEqual,
	CF_Greater,
	CF_GreaterEqual,
	CF_Equal,
	CF_NotEqual,
	CF_Never,
	CF_Always,

	// Utility enumerations
	//CF_DepthNearOrEqual = (((int32)ERHIZBuffer::IsInverted != 0) ? CF_GreaterEqual : CF_LessEqual),
	//CF_DepthNear = (((int32)ERHIZBuffer::IsInverted != 0) ? CF_Greater : CF_Less),
	//CF_DepthFartherOrEqual = (((int32)ERHIZBuffer::IsInverted != 0) ? CF_LessEqual : CF_GreaterEqual),
	//CF_DepthFarther = (((int32)ERHIZBuffer::IsInverted != 0) ? CF_Less : CF_Greater),
};

enum EStencilOp
{
	SO_Keep,
	SO_Zero,
	SO_Replace,
	SO_SaturatedIncrement,
	SO_SaturatedDecrement,
	SO_Invert,
	SO_Increment,
	SO_Decrement
};

struct FDepthStencilStateInitializerRHI
{
	bool bEnableDepthWrite;
	ECompareFunction DepthTest;

	bool bEnableFrontFaceStencil;
	ECompareFunction FrontFaceDepthTest;
	EStencilOp FrontFaceStencilFailStencilOp;
	EStencilOp FrontFaceDepthFailStencilOp;
	EStencilOp FrontFacePassStencilOp;

	bool bEnableBackFaceStencil;
	ECompareFunction BackFaceDepthTest;
	EStencilOp BackFaceStencilFailStencilOp;
	EStencilOp BackFaceDepthFailStencilOp;
	EStencilOp BackFacePassStencilOp;

	uint8 StencilReadMask;
	uint8 StencilWriteMask;
};

enum EBlendOperation
{
	BO_Add,
	BO_Subtract,
	BO_Min,
	BO_Max,
	BO_ReverseSubtract,
};

enum EBlendFactor
{
	BF_Zero,
	BF_One,
	BF_SourceColor,
	BF_InverseSourceColor,
	BF_SourceAlpha,
	BF_InverseSourceAlpha,
	BF_DestAlpha,
	BF_InverseDestAlpha,
	BF_DestColor,
	BF_InverseDestColor,
	BF_ConstantBlendFactor,
	BF_InverseConstantBlendFactor
};

enum EColorWriteMask
{
	CW_RED = 0x01,
	CW_GREEN = 0x02,
	CW_BLUE = 0x04,
	CW_ALPHA = 0x08,

	CW_NONE = 0,
	CW_RGB = CW_RED | CW_GREEN | CW_BLUE,
	CW_RGBA = CW_RED | CW_GREEN | CW_BLUE | CW_ALPHA,
	CW_RG = CW_RED | CW_GREEN,
	CW_BA = CW_BLUE | CW_ALPHA,
};

struct FBlendStateInitializerRHI
{
	struct RenderTargetBlendState
	{
		EBlendOperation ColorBlendOp;
		EBlendFactor ColorSrcBlend;
		EBlendFactor ColorDestBlend;
		EBlendOperation AlphaBlendOp;
		EBlendFactor AlphaSrcBlend;
		EBlendFactor AlphaDestBlend;
		EColorWriteMask ColorWriteMask;
	};

	RenderTargetBlendState RenderTargets[MaxSimultaneousRenderTargets];
	bool bUseIndependentRenderTargetBlendStates;
};

enum EVertexElementType
{
	VET_None,
	VET_Float1,
	VET_Float2,
	VET_Float3,
	VET_Float4,
	VET_PackedNormal,	// FPackedNormal
	VET_UByte4,
	VET_UByte4N,
	VET_Color,
	VET_Short2,
	VET_Short4,
	VET_Short2N,		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
	VET_Half2,			// 16 bit float using 1 bit sign, 5 bit exponent, 10 bit mantissa 
	VET_Half4,
	VET_Short4N,		// 4 X 16 bit word, normalized 
	VET_UShort2,
	VET_UShort4,
	VET_UShort2N,		// 16 bit word normalized to (value/65535.0,value/65535.0,0,0,1)
	VET_UShort4N,		// 4 X 16 bit word unsigned, normalized 
	VET_URGB10A2N,		// 10 bit r, g, b and 2 bit a normalized to (value/1023.0f, value/1023.0f, value/1023.0f, value/3.0f)
	VET_MAX
};

struct FVertexElement
{
	uint8 StreamIndex;
	uint8 Offset;
	EVertexElementType Type;
	uint8 AttributeIndex;
	uint16 Stride;

	uint16 bUseInstanceIndex;

	FVertexElement() {}
};

typedef std::vector<FVertexElement> FVertexDeclarationElementList;


struct FRHIResourceCreateInfo
{
	FRHIResourceCreateInfo() {}
};

struct FUpdateTextureRegion2D
{
	uint32 DestX;
	uint32 DestY;

	uint32 SrcX;
	uint32 SrcY;

	uint32 Wight;
	uint32 Height;

	FUpdateTextureRegion2D(uint32 InDestX, uint32 InDestY, uint32 InSrcX, uint32 InSrcY, uint32 InWight, uint32 InHeight)
		:DestX(InDestX),
		DestY(InDestY),
		SrcX(InSrcX),
		SrcY(InSrcY),
		Wight(InWight),
		Height(InHeight)
	{

	}

};

struct FUpdateTextureRegion3D
{
	uint32 DestX;
	uint32 DestY;
	uint32 DestZ;

	uint32 SrcX;
	uint32 SrcY;
	uint32 SrcZ;

	uint32 Wight;
	uint32 Height;
	uint32 Depth;

	FUpdateTextureRegion3D(uint32 InDestX, uint32 InDestY,uint32 InDestZ, uint32 InSrcX, uint32 InSrcY, uint32 InSrcZ, uint32 InWight, uint32 InHeight , uint32 InDepth)
		:DestX(InDestX),
		DestY(InDestY),
		DestZ(InDestZ),
		SrcX(InSrcX),
		SrcY(InSrcY),
		SrcZ(InSrcZ),
		Wight(InWight),
		Height(InHeight),
		Depth(InDepth)
	{

	}

};


enum class EResourceTransitionAccess
{
	EReadable, //transition from write-> read
	EWritable, //transition from read -> write	
	ERWBarrier, // Mostly for UAVs.  Transition to read/write state and always insert a resource barrier.
	ERWNoBarrier, //Mostly UAVs.  Indicates we want R/W access and do not require synchronization for the duration of the RW state.  The initial transition from writable->RWNoBarrier and readable->RWNoBarrier still requires a sync
	ERWSubResBarrier, //For special cases where read/write happens to different subresources of the same resource in the same call.  Inserts a barrier, but read validation will pass.  Temporary until we pass full subresource info to all transition calls.
	EMaxAccess,
};
enum class EResourceTransitionPipeline
{
	EGfxToCompute,
	EComputeToGfx,
	EGfxToGfx,
	EComputeToCompute,
};

struct FViewportBound
{
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;

	FViewportBound(float InTopLeftX,float InTopLeftY,float InWidth,float InHeight,float InMinDepth,float InMaxDepth)
		:TopLeftX(InTopLeftX)
		,TopLeftY(InTopLeftY)
		,Width(InWidth)
		,Height(InHeight)
		,MinDepth(InMinDepth)
		,MaxDepth(InMaxDepth)
	{

	}
};

struct FResolveRect
{
	int32 X1;
	int32 Y1;
	int32 X2;
	int32 Y2;

	FResolveRect(int32 InX1 = -1, int32 InY1 = -1, int32 InX2 = -1, int32 InY2 = -1)
		:X1(InX1)
		, X2(InX2)
		, Y1(InY1)
		, Y2(InY2)
	{

	}

	bool IsValid() const
	{
		return X1 >= 0 && Y1 >= 0 && X2 - X1 > 0 && Y2 - Y1 > 0;
	}
};

struct FResolveParams
{
	ECubeFace CubeFace;
	FResolveRect ResolveRect;
	int32 MipIndex;
	int32 SourceArrayIndex;
	int32 DestArrayIndex;
	FResolveParams(ECubeFace InCubeFace = CubeFace_PosX, const FResolveRect& InResolveRect = FResolveRect(), int32 InMipIndex = 0,
		int32 InSourceArrayIndex = 0,
		int32 InDestArrayIndex = 0)
		:CubeFace(InCubeFace)
		,ResolveRect(InResolveRect)
		,MipIndex(InMipIndex)
		,SourceArrayIndex(InSourceArrayIndex)
		,DestArrayIndex(InDestArrayIndex)
	{

	}
};