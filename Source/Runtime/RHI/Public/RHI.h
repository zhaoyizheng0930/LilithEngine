#pragma once
#include "HAL/PlatformIncludes.h"
#include "DynamicRHI.h"

/** Initializes the RHI. */
extern void RHIInit(bool bHasEditorToken);

/** Performs additional RHI initialization before the render thread starts. */
extern void RHIPostInit();

/** Shuts down the RHI. */
extern void RHIExit();

extern FDynamicRHI* CreateRHI();

extern std::wstring GRHIAdapterName;
extern uint32 GRHIVendorId = 0;
extern uint32 GRHIDeviceId = 0;

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

struct FBlendStateInitializerRHI
{

};