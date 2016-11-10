#pragma once

class FD3D11StateCacheBase
{
public:
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

protected:
	ID3D11DeviceContext* Direct3DDeviceIMContext;
private:
	// Viewport Cache
	uint32			CurrentNumberOfViewports;
	D3D11_VIEWPORT  CurrentViewport[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
};