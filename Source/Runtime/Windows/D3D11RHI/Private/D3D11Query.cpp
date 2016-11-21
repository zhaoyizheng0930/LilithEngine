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

bool FD3D11DynamicRHI::GetQueryData(ID3D11Query* Query, void* Data, SIZE_T DataSize, bool bWait, ERenderQueryType QueryType)
{
	//ZYZ_TODO:Support TimeOut Later
	HRESULT Result = Direct3DDeviceIMContext->GetData(Query, Data, DataSize, 0);
	if (Result == S_FALSE/* && bWait*/)
	{
		do
		{
			Result = Direct3DDeviceIMContext->GetData(Query, Data, DataSize, 0);

		} while (Result == S_FALSE);
	}
	else
	{
		return true;
	}
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

void FD3D11DynamicRHI::RHIBeginRenderQuery(FRHIRenderQuery* RenderQuery)
{
	FD3D11RenderQuery* D11RenderQuery = (FD3D11RenderQuery*)RenderQuery;

	if (D11RenderQuery->Querytype == RQT_Occlusion)
	{
		Direct3DDeviceIMContext->Begin(D11RenderQuery->Resource);
	}
}

void FD3D11DynamicRHI::RHIEndRenderQuery(FRHIRenderQuery* RenderQuery)
{
	FD3D11RenderQuery* D11RenderQuery = (FD3D11RenderQuery*)RenderQuery;

	if (D11RenderQuery->Querytype == RQT_Occlusion)
	{
		Direct3DDeviceIMContext->End(D11RenderQuery->Resource);
	}
}

void FD3D11DynamicRHI::RHIBeginOcclusionQueryBatch()
{

}

void FD3D11DynamicRHI::RHIEndOcclusionQueryBatch()
{

}