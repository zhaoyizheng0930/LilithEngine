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
	void SetRasterizerState(ID3D11RasterizerState* RasterizerState)
	{
		if (CurrentRasterizerState != RasterizerState);
		{
			CurrentRasterizerState = RasterizerState;
			Direct3DDeviceIMContext->RSSetState(RasterizerState);
		}
	}
	//Shader-----------------------------------------------------------------------VertexShader
	void SetVertexShader(ID3D11VertexShader* VertexShader)
	{
		//Compare Dirty
		if (CurrentVertexShader != VertexShader)
		{
			CurrentVertexShader = VertexShader;
			Direct3DDeviceIMContext->VSSetShader(VertexShader, nullptr, 0);
		}
	}
	//Shader------------------------------------------------------------------------HullShader
	void SetHullSahder(ID3D11HullShader* HullShader)
	{
		//Compare Dirty
		if (CurrentHullShader != HullShader)
		{
			CurrentHullShader = HullShader;
			Direct3DDeviceIMContext->HSSetShader(HullShader);
		}
	}
	//Shader------------------------------------------------------------------------Domain
	void SetDomainShader(ID3D11DomainShader* DomainShader)
	{
		//Compare Dirty
		if (CurrentDoaminShader != DomainShader)
		{
			CurrentDoaminShader = DomainShader;
			Direct3DDeviceIMContext->DSSetShader(DomainShader , nullptr , 0);
		}
	}
	//Shader------------------------------------------------------------------------Geometry
	void SetGeometryShader(ID3D11GeometryShader* GeometryShader)
	{
		//Compare Dirty
		if (CurrentGeometryShader != GeometryShader)
		{
			CurrentGeometryShader = GeometryShader;
			Direct3DDeviceIMContext->GSSetShader(GeometryShader, nullptr, 0);
		}
	}
	//Shader------------------------------------------------------------------------Pixel
	void SetPixelShader(ID3D11PixelShader* PixelShader)
	{
		//Compare Dirty
		if (CurrentPixelShader != PixelShader)
		{
			CurrentPixelShader = PixelShader;
			Direct3DDeviceIMContext->PSSetShader(CurrentPixelShader, nullptr, 0);
		}
	}
	//Shader------------------------------------------------------------------------Compute
	void SetComputeShader(ID3D11ComputeShader* ComputeShader)
	{
		if (CurrentComputeShader != ComputeShader)
		{
			CurrentComputeShader = ComputeShader;
			Direct3DDeviceIMContext->CSSetShader(ComputeShader, nullptr, 0);
		}
	}

	//Stream Source--------------------------------------------------------------------------
	void SetStreamSource(ID3D11Buffer* VertexBuffer ,uint32 StreamIndex , uint32 Stride , uint32 Offset )
	{
		//Cahce State
		InternalSetStreamSource(VertexBuffer , StreamIndex , Stride , Offset , nullptr);
	}

	//Index Buffer--------------------------------------------------------------------------
	void SetIndexBuffer(ID3D11Buffer* IndexBuffer,DXGI_FORMAT Format , uint32 Offset)
	{
		InternalSetIndeBuffer(IndexBuffer , Format , Offset , nullptr);
	}

	//Inpute Layout---------------------------------------------------------------------------
	void SetInputLayout(ID3D11InputLayout* InputeLayout)
	{
		if (CurrentInputLayout != InputeLayout)
		{
			CurrentInputLayout = InputeLayout;
			Direct3DDeviceIMContext->IASetInputLayout(InputeLayout);
		}
	}

	//Viewport--------------------------------------------------------------------------
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

	void SetViewport(D3D11_VIEWPORT Viewport)
	{
		if (CurrentNumberOfViewports != 1 || FMemory::Memcmp(&CurrentViewport[0], &Viewports, sizeof(D3D11_VIEWPORT)))
		{
			FMemory::Memcpy(&CurrentViewport[0], &Viewports, sizeof(D3D11_VIEWPORT));
			CurrentNumberOfViewports = 1;
			Direct3DDeviceIMContext->RSSetViewports(1, &Viewport);
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
	ID3D11HullShader* CurrentHullShader;
	ID3D11DomainShader* CurrentDoaminShader;
	ID3D11GeometryShader* CurrentGeometryShader;
	ID3D11PixelShader* CurrentPixelShader;
	ID3D11ComputeShader* CurrentComputeShader;
	// Viewport Cache
	uint32			CurrentNumberOfViewports;
	D3D11_VIEWPORT  CurrentViewport[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

	//VertexBuffer Cache
	struct FD3D11VertexBufferState
	{
		ID3D11Buffer* VertexBuffer;
		uint32 Stride;
		uint32 Offset;
	}CurrentVertexBuffer[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];

	//IndexBuffer Cache
	struct FD3D11IndexBufferState
	{
		ID3D11Buffer* IndexBuffer;
		DXGI_FORMAT Format;
		uint32 Offset;
	}CurrentIndexBuffer;

	//InputLayout Cache
	ID3D11InputLayout* CurrentInputLayout;

	//StateCache
	ID3D11RasterizerState* CurrentRasterizerState;

private:

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

	typedef void(*TSetStreamSourceAlternate)(FD3D11StateCacheBase* StateCache, ID3D11Buffer* VertexBuffer, uint32 StreamIndex, uint32 Stride, uint32 Offset);
	void InternalSetStreamSource(ID3D11Buffer* VertexBuffer, uint32 StreamIndex, uint32 Stride, uint32 Offset, TSetStreamSourceAlternate AlternatePathFunction)
	{
		FD3D11VertexBufferState& CacheVertexBuffer = CurrentVertexBuffer[StreamIndex];
		//Dirty Compare
		if (CacheVertexBuffer.VertexBuffer != VertexBuffer || CacheVertexBuffer.Stride != Stride || CacheVertexBuffer.Offset != Offset)
		{
			CacheVertexBuffer.VertexBuffer = VertexBuffer;
			CacheVertexBuffer.Stride = Stride;
			CacheVertexBuffer.Offset = Offset;
			if (AlternatePathFunction != NULL)
			{
				AlternatePathFunction(this, VertexBuffer, StreamIndex, Stride, Offset);
			}
			else
			{
				Direct3DDeviceIMContext->IASetVertexBuffers(StreamIndex, 1, &VertexBuffer, &Stride, &Offset);
			}
		}
	}

	typedef void(*TSetIndexBufferAlternate)(FD3D11StateCacheBase* StateCache, ID3D11Buffer* IndexBuffer, DXGI_FORMAT Format, uint32 Offset);
	void InternalSetIndeBuffer(ID3D11Buffer* IndexBuffer, DXGI_FORMAT Format, uint32 Offset , TSetIndexBufferAlternate AlternatePathFunction)
	{
		//Compare Dirty
		if (CurrentIndexBuffer.IndexBuffer != IndexBuffer || CurrentIndexBuffer.Offset != Offset || CurrentIndexBuffer.Format != Format)
		{
			CurrentIndexBuffer.IndexBuffer = IndexBuffer;
			CurrentIndexBuffer.Offset = Offset;
			CurrentIndexBuffer.Format = Format;
			if (AlternatePathFunction == NULL)
			{
				Direct3DDeviceIMContext->IASetIndexBuffer(IndexBuffer, Format, Offset);
			}
			else
			{
				AlternatePathFunction(this, IndexBuffer, Format, Offset);
			}
		}
	}
};