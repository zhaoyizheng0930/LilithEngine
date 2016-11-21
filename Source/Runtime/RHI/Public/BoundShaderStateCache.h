#pragma once
#include "RHIResource.h"

///** Lazily initialized bound shader state cache singleton. */
//static FBoundShaderStateCache_Threadsafe& GetBoundShaderStateCache_Threadsafe()
//{
//	return GBoundShaderStateCache_ThreadSafe;
//}

class FBoundShaderStateKey
{
public:
	FBoundShaderStateKey(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIPixelShader* InPixelShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader)
		:VertexDeclaration(InVertexDeclaration),
		VertexShader(InVertexShader),
		PixelShader(InPixelShader),
		HullShader(InHullShader),
		DomainShader(InDomainShader),
		GeometryShader(InGeometryShader)
	{

	}

	FRHIVertexShader*   GetVertexShader() const { return VertexShader; }
	FRHIPixelShader*    GetPixelShader() const { return PixelShader; }
	FRHIHullShader*     GetHullShader() const { return HullShader; }
	FRHIDomainShader*   GetDomainShader() const { return DomainShader; }
	FRHIGeometryShader* GetGeometryShader() const { return GeometryShader; }

	bool operator ==(const FBoundShaderStateKey& A)
	{
		return A.VertexDeclaration == VertexDeclaration&&
			A.VertexShader == VertexShader&&
			A.PixelShader == PixelShader&&
			A.HullShader == HullShader&&
			A.DomainShader == DomainShader&&
			A.GeometryShader == GeometryShader;
	}

protected:
private:

	FRHIVertexDeclaration* VertexDeclaration;
	FRHIVertexShader* VertexShader;
	FRHIPixelShader* PixelShader;
	FRHIHullShader* HullShader;
	FRHIDomainShader* DomainShader;
	FRHIGeometryShader* GeometryShader;
};

class FCachedBoundShaderStateLink
{
public:
	FCachedBoundShaderStateLink(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIPixelShader* InPixelShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader, FRHIBoundShaderState* InBoundShaderState, bool bAddToSingleThreadedCache = true);

	virtual ~FCachedBoundShaderStateLink();

	FRHIVertexShader* GetVertexShader() { return (FRHIVertexShader*)Key.GetVertexShader(); }
	FRHIPixelShader* GetPixelShader() { return (FRHIPixelShader*)Key.GetVertexShader(); }
	FRHIHullShader* GetHullShader() { return (FRHIHullShader*)Key.GetVertexShader(); }
	FRHIDomainShader* GetDomainShader() { return (FRHIDomainShader*)Key.GetVertexShader(); }
	FRHIGeometryShader* GetGeometryShader() { return (FRHIGeometryShader*)Key.GetVertexShader(); }

protected:
private:
	FRHIBoundShaderState* BoundShaderState;

	FBoundShaderStateKey Key;

	bool bAddToSingleThreadCache;
};
