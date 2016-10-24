#pragma once
#include "RHIResource.h"


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

class FD3D11DepthStencilState :public FRHIDepthStencilState
{
public:
	ID3D11DepthStencilState* Resource;
protected:
private:
};

class FD3D11BlendState :public FRHIBlendState
{
public:
	ID3D11BlendState* Resource;
protected:
private:
};