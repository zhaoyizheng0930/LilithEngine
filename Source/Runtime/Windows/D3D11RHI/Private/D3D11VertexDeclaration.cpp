#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"

struct FD3D11VertexDeclarationKey
{
	/** Vertex elements in the declaration. */
	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexElements;
	/** Hash of the vertex elements. */
	uint32 Hash;

	FD3D11VertexDeclarationKey(const std::vector<FVertexElement>& InVertexElements)
	{
		for (int i = 0; i < InVertexElements.size();i++)
		{
			FVertexElement vertexEle = InVertexElements[i];
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.InputSlot = vertexEle.StreamIndex;
			desc.AlignedByteOffset = vertexEle.Offset;
			switch (vertexEle.Type)
			{
			case VET_Float1:			desc.Format = DXGI_FORMAT_R32_FLOAT; break;
			case VET_Float2:			desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
			case VET_Float3:			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case VET_Float4:			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			case VET_PackedNormal:		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
			case VET_UByte4:			desc.Format = DXGI_FORMAT_R8G8B8A8_UINT; break;
			case VET_UByte4N:			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
			case VET_Color:				desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; break;
			case VET_Short2:			desc.Format = DXGI_FORMAT_R16G16_SINT; break;
			case VET_Short4:			desc.Format = DXGI_FORMAT_R16G16B16A16_SINT; break;
			case VET_Short2N:			desc.Format = DXGI_FORMAT_R16G16_SNORM; break;
			case VET_Half2:				desc.Format = DXGI_FORMAT_R16G16_FLOAT; break;
			case VET_Half4:				desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
			case VET_Short4N:			desc.Format = DXGI_FORMAT_R16G16B16A16_SNORM; break;
			case VET_UShort2:			desc.Format = DXGI_FORMAT_R16G16_UINT; break;
			case VET_UShort4:			desc.Format = DXGI_FORMAT_R16G16B16A16_UINT; break;
			case VET_UShort2N:			desc.Format = DXGI_FORMAT_R16G16_UNORM; break;
			case VET_UShort4N:			desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM; break;
			case VET_URGB10A2N:			desc.Format = DXGI_FORMAT_R10G10B10A2_UNORM; break;
			default:
				break;
			}
			desc.SemanticName = "ATTRIBUTE";
			desc.SemanticIndex = vertexEle.AttributeIndex;
			desc.InputSlotClass = vertexEle.bUseInstanceIndex? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = vertexEle.bUseInstanceIndex ? 1 : 0;

			VertexElements.push_back(desc);
		}

		struct FCompareDesc
		{
			bool operator()(const D3D11_INPUT_ELEMENT_DESC& A, const D3D11_INPUT_ELEMENT_DESC &B) const
			{
				return ((int32)A.AlignedByteOffset + A.InputSlot * ((uint16)0xffff)) < ((int32)B.AlignedByteOffset + B.InputSlot * ((uint16)0xffff));
			}
		};

		sort(VertexElements.begin(), VertexElements.end(), FCompareDesc());

		//Todo:Generate Hash.

	}
};


FRHIVertexDeclaration* FD3D11DynamicRHI::RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements)
{
	//Use Hash test.TODO!!!!!!!!!!!!!!!!!!!!!!!Memory leak
	FD3D11VertexDeclarationKey key(Elements);

	FD3D11VertexDeclaration* VertextDecl = nullptr;
	VertextDecl = new FD3D11VertexDeclaration(key.VertexElements);
	return VertextDecl;
}