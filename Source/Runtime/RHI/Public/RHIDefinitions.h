#pragma once

#define MaxSimultaneousRenderTargets 8

enum EResourceLockMode
{
	RLM_ReadOnly,
	RLM_WriteOnly,
	RLM_Num
};