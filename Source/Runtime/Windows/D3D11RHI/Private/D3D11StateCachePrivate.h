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
protected:
	ID3D11DeviceContext* Direct3DDeviceIMContext;
private:
};