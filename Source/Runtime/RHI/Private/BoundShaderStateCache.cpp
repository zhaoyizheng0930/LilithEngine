#include "RHIPCH.h"
#include "BoundShaderStateCache.h"

typedef std::map<FBoundShaderStateKey, FCachedBoundShaderStateLink*> FBoundShaderStateCacheMap;

static FBoundShaderStateCacheMap GBoundShaderStateCache;

static FBoundShaderStateCacheMap& GetBoundShaderStateCache()
{
	return GBoundShaderStateCache;
}

FCachedBoundShaderStateLink::FCachedBoundShaderStateLink(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIPixelShader* InPixelShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader, FRHIBoundShaderState* InBoundShaderState, bool bAddToSingleThreadedCache)
	:Key(InVertexDeclaration, InVertexShader, InPixelShader, InHullShader, InDomainShader, InGeometryShader),
	BoundShaderState(InBoundShaderState),
	bAddToSingleThreadCache(bAddToSingleThreadedCache)
{
	if (bAddToSingleThreadCache)
	{
		GetBoundShaderStateCache().insert(std::make_pair(Key, this));
	}
}

FCachedBoundShaderStateLink::~FCachedBoundShaderStateLink()
{
	if (bAddToSingleThreadCache)
	{
		GetBoundShaderStateCache().erase(Key);
		bAddToSingleThreadCache = false;
	}
}