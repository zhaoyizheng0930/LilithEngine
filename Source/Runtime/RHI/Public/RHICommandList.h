#pragma once

class FRHICommandListBase
{
public:
protected:
private:
};

class FRHICommandList : public FRHICommandListBase
{
public:
protected:
private:
};

class FRHICommandList_RecursiveHazardous : public FRHICommandList
{
	FRHICommandList_RecursiveHazardous()
	{

	}
public:
	//FRHICommandList_RecursiveHazardous(IRHICommandContext *Context)
	//{
	//	SetContext(Context);
	//}
};