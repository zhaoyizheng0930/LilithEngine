#pragma once
#include "RHIResource.h"

typedef std::map<FBoundShaderStateKey, FCachedBoundShaderStateLink*> FBoundShaderStateCacheMap;

extern FBoundShaderStateCacheMap GBoundShaderStateCache;

static FBoundShaderStateCacheMap& GetBoundShaderStateCache()
{
	return GBoundShaderStateCache;
}

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
	FCachedBoundShaderStateLink(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIPixelShader* InPixelShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader, FD3D11BoundShaderState* InBoundShaderState, bool bAddToSingleThreadedCache = true)
		:Key(InVertexDeclaration, InVertexShader, InPixelShader, InHullShader, InDomainShader, InGeometryShader),
		BoundShaderState(InBoundShaderState),
		bAddToSingleThreadCache(bAddToSingleThreadedCache)
	{
		if (bAddToSingleThreadCache)
		{
			GetBoundShaderStateCache().insert(std::make_pair(Key, this));
		}
	}

	~FCachedBoundShaderStateLink()
	{
		if (bAddToSingleThreadCache)
		{
			GetBoundShaderStateCache().erase(Key);
			bAddToSingleThreadCache = false;
		}
	}

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