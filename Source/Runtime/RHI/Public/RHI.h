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
	SF_Point,				//???:???????
	SF_Bilinear,			//???:???Level?4??????????
	SF_Trilinear,			//???:??level?????????
	SF_AnisotropicPoint,	//????:??????????????????????????????
	SF_AnisotropicLinear,
};
//
//Mip:??mip map
//Min: ????
//Mag: ????

enum ESamplerAddressMode	//UV????0 1?????
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

enum ERasterizerFillMode
{
	FM_Point,
	FM_Wireframe,
	FM_Solid
};

enum ERasterizerCullMode
{
	CM_None,
	CM_CW,
	CM_CCW
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

struct FDepthStencilStateInitializerRHI
{

};

struct FBlendStateInitializerRHI
{

};