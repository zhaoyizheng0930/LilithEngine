#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "D3D11Viewport.h"

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


void FD3D11EventQuery::IssueEvent()
{
	D3DRHI->GetContext()->End(Query);
}

void FD3D11EventQuery::WaitForCompletion()
{
	bool bRenderIsFinished = false;
	while (D3DRHI->GetContext()->GetData(Query , &bRenderIsFinished ,sizeof(bRenderIsFinished),RQT_Undefined ) && !bRenderIsFinished)
	{
	}
}

// FRenderResource interface.
void FD3D11EventQuery::InitDynamicRHI()
{
	//Create Query
	D3D11_QUERY_DESC desc;
	desc.Query = D3D11_QUERY_EVENT;
	desc.MiscFlags = 0;

	D3DRHI->GetDevice()->CreateQuery(&desc, &Query);

	IssueEvent();
}

void FD3D11EventQuery::ReleaseDynamicRHI()
{
	Query = NULL;
}