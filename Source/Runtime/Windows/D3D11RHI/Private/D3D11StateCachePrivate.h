#pragma once
#include "RHIDefinitions.h"
#include "HAL/Memory/LilithMemory.h"

class FD3D11StateCacheBase
{
public:
	enum ESRV_Type
	{
		SRV_Unknown,
		SRV_Dynamic,
		SRV_Static,
	};

	FD3D11StateCacheBase() {}
	~FD3D11StateCacheBase() {}

	void Init(FD3D11DeviceContext* InDeviceContext)
	{
		SetContext(InDeviceContext);
	}

	void SetContext(FD3D11DeviceContext* InDeviceContext)
	{
		Direct3DDeviceIMContext = InDeviceContext;
		ClearState();
	}

	void ClearState()
	{

	}

	//StateSet---------------------------------------------------------------------------------
	//Shader
	void SetVertexShader(ID3D11VertexShader* VertexShader)
	{
		//Compare Dirty
		if (CurrentVertexShader != VertexShader)
		{
			Direct3DDeviceIMContext->VSSetShader(VertexShader, nullptr, 0);
		}
	}

	//Viewport
	void SetViewports(uint32 Count, D3D11_VIEWPORT* Viewports)
	{
		//Compare Dirty
		if (Count != CurrentNumberOfViewports ||FMemory::Memcmp(&CurrentViewport[0] , Viewports , Count * sizeof(D3D11_VIEWPORT)))
		{
			FMemory::Memcpy(&CurrentViewport[0], Viewports, Count * sizeof(D3D11_VIEWPORT));
			CurrentNumberOfViewports = Count;
			Direct3DDeviceIMContext->RSSetViewports(Count, Viewports);
		}
	}

	template <EShaderFrequency ShaderFrequency>
	void SetSahderResourceView(ID3D11ShaderResourceView* SRV, uint32 ResourceIndex, ESRV_Type SrvType = SRV_Unknown)
	{
		InternalSetSahderResourceView<ShaderFrequency>(SRV , ResourceIndex);
	}

protected:
	ID3D11DeviceContext* Direct3DDeviceIMContext;
private:
	//Shader Cache
	ID3D11VertexShader* CurrentVertexShader;
	// Viewport Cache
	uint32			CurrentNumberOfViewports;
	D3D11_VIEWPORT  CurrentViewport[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];


	template <EShaderFrequency ShaderFrequency>
	void InternalSetSahderResourceView(ID3D11ShaderResourceView* SRV, uint32 ResourceIndex)
	{
		switch (ShaderFrequency)
		{
		case SF_Vertex:		Direct3DDeviceIMContext->VSSetShaderResources(ResourceIndex, 1, &SRV); break;
		case SF_Hull:		Direct3DDeviceIMContext->HSSetShaderResources(ResourceIndex, 1, &SRV); break;
		case SF_Domain:		Direct3DDeviceIMContext->DSSetShaderResources(ResourceIndex, 1, &SRV); break;
		case SF_Geometry:	Direct3DDeviceIMContext->GSSetShaderResources(ResourceIndex, 1, &SRV); break;
		case SF_Pixel:		Direct3DDeviceIMContext->PSSetShaderResources(ResourceIndex, 1, &SRV); break;
		case SF_Compute:	Direct3DDeviceIMContext->CSSetShaderResources(ResourceIndex, 1, &SRV); break;
		}
	}
};