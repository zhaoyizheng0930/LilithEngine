#pragma once
#include "RHIDefinitions.h"
#include "HAL/Memory/LilithMemory.h"
#include "RHI.h"

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

	void GetRasterizerState(ID3D11RasterizerState** RasterizerState)
	{
		*RasterizerState = CurrentRasterizerState;
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

	void GetVertexShader(ID3D11VertexShader** VertexShader)
	{
		//Compare Dirty
		*VertexShader = CurrentVertexShader;
	}
	//Shader------------------------------------------------------------------------HullShader
	void SetHullSahder(ID3D11HullShader* HullShader)
	{
		//Compare Dirty
		if (CurrentHullShader != HullShader)
		{
			CurrentHullShader = HullShader;
			Direct3DDeviceIMContext->HSSetShader(HullShader, nullptr, 0);
		}
	}

	void GetHullSahder(ID3D11HullShader** HullShader)
	{
		*HullShader = CurrentHullShader;
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

	void GetDomainShader(ID3D11DomainShader** DomainShader)
	{
		*DomainShader = CurrentDoaminShader;
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

	void GetGeometryShader(ID3D11GeometryShader** GeometryShader)
	{
		*GeometryShader = CurrentGeometryShader
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

	void GetPixelShader(ID3D11PixelShader** PixelShader)
	{
		*PixelShader = CurrentPixelShader;
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

	void SetComputeShader(ID3D11ComputeShader** ComputeShader)
	{
		*ComputeShader = CurrentComputeShader;
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

	void GetInputLayout(ID3D11InputLayout** InputeLayout)
	{
		*InputeLayout = CurrentInputLayout;
	}
	//ConstantBuffer
	template<EShaderFrequency ShaderFrequency>
	void SetConstantBuffer(ID3D11Buffer* CanstantBuffer , uint32 SlotIndex)
	{
		FD3D11ConstantBufferState& CBuffer = CurrentConstantBuffers[ShaderFrequency][SlotIndex].Buffer
		if (CBuffer.Buffer != CanstantBuffer || CBuffer.FirstConstant != 0 || CBuffer.NumConstants != D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT)
		{
			CBuffer.Buffer = CanstantBuffer;
			CBuffer.FirstConstant = 0;
			CBuffer.NumConstants = D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT;
			InternalSetSetConstantBuffer<ShaderFrequency>(CanstantBuffer, SlotIndex);
		}
	}

	template<EShaderFrequency ShaderFrequency>
	void GetConstantBuffer(uint32 SlotIndex , uint32 ResourceCount, ID3D11Buffer** CanstantBuffer)
	{
		for (uint32 i = 0; i < ResourceCount;i++)
		{
			CanstantBuffer[i] = CurrentConstantBuffers[i + SlotIndex].Buffer;
		}
	}

	//SetPrimitiveTopology
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology)
	{
		if (CurrentPrimitiveTopology != PrimitiveTopology)
		{
			CurrentPrimitiveTopology = PrimitiveTopology;
			Direct3DDeviceIMContext->IASetPrimitiveTopology(CurrentPrimitiveTopology);
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
		if (CurrentNumberOfViewports != 1 || FMemory::Memcmp(&CurrentViewport[0], (void*)&Viewport, sizeof(D3D11_VIEWPORT)))
		{
			FMemory::Memcpy(&CurrentViewport[0], &Viewport, sizeof(D3D11_VIEWPORT));
			CurrentNumberOfViewports = 1;
			Direct3DDeviceIMContext->RSSetViewports(1, &Viewport);
		}
	}

	D3D11_VIEWPORT* GetViewports(uint32* Count, D3D11_VIEWPORT *Viewports)
	{
		if (Viewports)
		{
			uint32 NeedCopyCount = (uint32)*Count;
			uint32 CopyNumberOfViewport = FMath::Min(NeedCopyCount, CurrentNumberOfViewports);
			CopyNumberOfViewport = FMath::Min(CopyNumberOfViewport, (uint32)D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE);
			if (CopyNumberOfViewport > 0)
			{
				FMemory::Memcpy(Viewports, Viewports, sizeof(D3D11_VIEWPORT) *CopyNumberOfViewport);
			}
			if (NeedCopyCount > CopyNumberOfViewport)
			{
				FMemory::Memset(&Viewports[CopyNumberOfViewport], 0, sizeof(D3D11_VIEWPORT) *(NeedCopyCount - CopyNumberOfViewport));
			}
		}
		*Count = CurrentNumberOfViewports;
	}

	//SRV--------------------------------------------------------------------------
	template <EShaderFrequency ShaderFrequency>
	void SetShaderResourceView(ID3D11ShaderResourceView* SRV, uint32 ResourceIndex, ESRV_Type SrvType = SRV_Unknown)
	{
		InternalSetShaderResourceView<ShaderFrequency>(SRV , ResourceIndex, nullptr);
	}

	template <EShaderFrequency ShaderFrequency>
	void GetShaderResourceView(uint32 StartResourceIndex, uint32 NumResources, ID3D11ShaderResourceView** SRV)
	{
		for (int i = 0; i < NumResources;i++)
		{
			SRV[i] = CurrentShaderResourceViews[StartResourceIndex + i];
		}
	}
	//State--------------------------------------------------------------------------Sampler
	template <EShaderFrequency ShaderFrequency>
	void SetSamplerState(ID3D11SamplerState* SamplerState , uint32 SamplerIndex)
	{
		InternalSetSamplerState<ShaderFrequency>(SamplerState, SamplerIndex, nullptr);
	}

	//State--------------------------------------------------------------------------DepthState
	void SetDepthStencilState(ID3D11DepthStencilState* DepthStencilState, uint32 RefStencil)
	{
		if (CurrentDepthStencilState != DepthStencilState || CurrentReferenceStencil != RefStencil)
		{
			CurrentDepthStencilState = DepthStencilState;
			CurrentReferenceStencil = RefStencil;
			Direct3DDeviceIMContext->OMSetDepthStencilState(DepthStencilState, RefStencil);
		}
	}

	void GetDepthStencilState(ID3D11DepthStencilState** DepthStencilState , uint32* StencilRef)
	{
		*DepthStencilState = CurrentDepthStencilState;
		*StencilRef = CurrentReferenceStencil;
	}

	//State---------------------------------------------------------------------------BlendState
	void SetBlendState(ID3D11BlendState* BlendState , const float Color[4] , uint32 SampleMask)
	{
		if (CurrentBlendState != BlendState || CurrentBlendSampleMask != CurrentBlendSampleMask || FMemory::Memcmp(CurrentBlendFactor , Color , sizeof(CurrentBlendFactor)))
		{
			CurrentBlendState = BlendState;
			CurrentBlendSampleMask = SampleMask;
			FMemory::Memcpy(CurrentBlendFactor, Color, sizeof(CurrentBlendFactor));
			Direct3DDeviceIMContext->OMSetBlendState(BlendState, Color, SampleMask);
		}
	}

	void GetBlendState(ID3D11BlendState** BlendState, const float Color[4], uint32* SampleMask)
	{
		*BlendState = CurrentBlendState;
		FMemory::Memcmp(Color, CurrentBlendFactor, sizeof(CurrentBlendFactor));
		*SampleMask = CurrentBlendSampleMask;
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

	// Shader Resource Views Cache
	ID3D11ShaderResourceView* CurrentShaderResourceViews[SF_NumFrequencies][D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];

	// Shader Sampler State Cache
	ID3D11SamplerState* CurrentSamplerStates[SF_NumFrequencies][D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];

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

	//Constant Buffer
	struct FD3D11ConstantBufferState
	{
		ID3D11Buffer* Buffer;
		uint32 FirstConstant;
		uint32 NumConstants;
	} CurrentConstantBuffers[SF_NumFrequencies][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];

	//D3D11_PRIMITIVE_TOPOLOGY
	D3D11_PRIMITIVE_TOPOLOGY CurrentPrimitiveTopology;

	//StateCache
	//RasterizerState
	ID3D11RasterizerState* CurrentRasterizerState;
	//DepthStencilState
	ID3D11DepthStencilState* CurrentDepthStencilState;
	uint32 CurrentReferenceStencil;
	//BlendState
	ID3D11BlendState* CurrentBlendState;
	uint32 CurrentBlendSampleMask;
	float CurrentBlendFactor[4];
private:

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
			if (AlternatePathFunction != nullptr)
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
			if (AlternatePathFunction == nullptr)
			{
				Direct3DDeviceIMContext->IASetIndexBuffer(IndexBuffer, Format, Offset);
			}
			else
			{
				AlternatePathFunction(this, IndexBuffer, Format, Offset);
			}
		}
	}

	typedef void(*TSetSahderResourceViewAlternate)(ID3D11ShaderResourceView* SRV, uint32 SamplerIndex);
	template <EShaderFrequency ShaderFrequency>
	void InternalSetShaderResourceView(ID3D11ShaderResourceView* SRV, uint32 SamplerIndex, TSetSahderResourceViewAlternate AlternatePathFunction)
	{
		//Compare Dirty
		if (CurrentShaderResourceViews[ShaderFrequency][SamplerIndex] != SRV)
		{
			CurrentShaderResourceViews[ShaderFrequency][SamplerIndex] = SRV;
			if (AlternatePathFunction == nullptr)
			{
				InternalSetShaderResourceView<ShaderFrequency>(SRV, SamplerIndex);
			}
			else
			{
				AlternatePathFunction(SRV , SamplerIndex);
			}
		}
	}

	typedef void(*TSetSamplerStateAlternate)(ID3D11SamplerState* SamplerState, uint32 ResourceIndex);
	template <EShaderFrequency ShaderFrequency>
	void InternalSetSamplerState(ID3D11SamplerState* SamplerState, uint32 ResourceIndex, TSetSamplerStateAlternate AlternatePathFunction)
	{
		//Compare Dirty
		if (CurrentSamplerStates[ShaderFrequency][ResourceIndex] != SamplerState)
		{
			CurrentSamplerStates[ShaderFrequency][ResourceIndex] = SamplerState;
			if (AlternatePathFunction == nullptr)
			{
				InternalSetSamplerState<ShaderFrequency>(SamplerState, ResourceIndex);
			}
			else
			{
				AlternatePathFunction(SamplerState , ResourceIndex);
			}
		}
	}

private:
	//Shader Template
	template <EShaderFrequency ShaderFrequency>
	void InternalSetShaderResourceView(ID3D11ShaderResourceView* SRV, uint32 ResourceIndex)
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

	template <EShaderFrequency ShaderFrequency>
	void InternalSetSamplerState(ID3D11SamplerState* SamplerState, uint32 SamplerIndex)
	{
		//Compare Dirty
		switch (ShaderFrequency)
		{
		case SF_Vertex:		Direct3DDeviceIMContext->VSSetSamplers(SamplerIndex, 1, &SamplerState); break;
		case SF_Hull:		Direct3DDeviceIMContext->HSSetSamplers(SamplerIndex, 1, &SamplerState); break;
		case SF_Domain:		Direct3DDeviceIMContext->DSSetSamplers(SamplerIndex, 1, &SamplerState); break;
		case SF_Geometry:	Direct3DDeviceIMContext->GSSetSamplers(SamplerIndex, 1, &SamplerState); break;
		case SF_Pixel:		Direct3DDeviceIMContext->PSSetSamplers(SamplerIndex, 1, &SamplerState); break;
		case SF_Compute:	Direct3DDeviceIMContext->CSSetSamplers(SamplerIndex, 1, &SamplerState); break;
		}
	}

	template <EShaderFrequency ShaderFrequency>
	void InternalSetSetConstantBuffer(ID3D11Buffer*& ConstantBuffer , uint32 SlotIndex)
	{
		//Compare Dirty
		switch (ShaderFrequency)
		{
		case SF_Vertex:		Direct3DDeviceIMContext->VSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer) break;
		case SF_Hull:		Direct3DDeviceIMContext->HSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
		case SF_Domain:		Direct3DDeviceIMContext->DSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
		case SF_Geometry:	Direct3DDeviceIMContext->GSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
		case SF_Pixel:		Direct3DDeviceIMContext->PSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
		case SF_Compute:	Direct3DDeviceIMContext->CSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
		}
	}

};