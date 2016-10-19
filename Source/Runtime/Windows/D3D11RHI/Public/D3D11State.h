#pragma once


class FD3D11SamplerState :public FRHISamplerState
{
public:
	ID3D11SamplerState* Resource;
protected:
private:
};

class FD3D11RasterizerState :public FRHIRasterizerState
{
public:
	ID3D11RasterizerState* Resource;
protected:
private:
};