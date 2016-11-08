#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"

FRHIRenderQuery* FD3D11DynamicRHI::RHICreateRenderQuery(ERenderQueryType QueryType)
{
	ID3D11Query* Query;
	D3D11_QUERY_DESC QueryDesc;
	if (QueryType == RQT_Occlusion)
	{
		QueryDesc.Query = D3D11_QUERY_OCCLUSION;
	}
	else if (QueryType == RQT_AbsoluteTime)
	{
		QueryDesc.Query = D3D11_QUERY_TIMESTAMP;
	}
	QueryDesc.MiscFlags = 0;

	Direct3DDevice->CreateQuery(&QueryDesc , &Query);
	return new FD3D11RenderQuery(Query , QueryType);
}

bool FD3D11DynamicRHI::RHIGetRenderQueryResult(FRHIRenderQuery* RenderQuery, uint64& OutResult, bool bWait)
{
	FD3D11RenderQuery* D11RenderQuery = (FD3D11RenderQuery*)RenderQuery;

	bool bSuccess = true;
	if (!D11RenderQuery->bResultIsCached)
	{
		bSuccess = GetQueryData(D11RenderQuery->Resource , &D11RenderQuery->Result , sizeof(D11RenderQuery->Result) , true , D11RenderQuery->Querytype);
		D11RenderQuery->bResultIsCached = bSuccess;
	}

	if (D11RenderQuery->Querytype == RQT_AbsoluteTime)
	{
		//ZYZ_TODO:didn't understand.Support later.
	}
	else
	{
		OutResult = D11RenderQuery->Result;
	}
	return bSuccess;
}