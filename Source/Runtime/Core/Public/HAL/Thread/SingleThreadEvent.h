#pragma once
#include "Event.h"

class FSingleThreadEvent :public FEvent
{
public:
	FSingleThreadEvent() {}
	~FSingleThreadEvent() {}

	bool Create(bool bIsManualReset = false)
	{
		bManualReset = bIsManualReset;
		return true;
	}

	bool IsManualReset()
	{
		return bManualReset;
	}

	void Trigger()
	{
		bTriggered = true;
	}

	void Reset()
	{
		bTriggered = false;
	}

	bool Wait(uint32 WaitTime, const bool bIgnoreThreadIdleStats = false)
	{
		bTriggered = bManualReset;
		return true;
	}
protected:
private:
	bool bTriggered;

	bool bManualReset;
};