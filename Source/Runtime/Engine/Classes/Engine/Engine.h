#pragma once
#include "Core.h"
#include "../../Public/UnrealEngine.h"

class UEngine
{
public:
	UEngine();

	void Init(IEngineLoop* InEngineLoop);

	void StartHardwareSurvey();

	void PreExit();
private:
};