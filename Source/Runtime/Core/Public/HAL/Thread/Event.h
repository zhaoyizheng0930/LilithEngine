#pragma once
#include "../../Core.h"

#define MAX_uint32		((uint32)	0xffffffff)

class FEvent
{
public:
	virtual bool Create(bool bIsManualReset = false) = 0;

	virtual bool IsManualReset() = 0;

	virtual void Trigger() = 0;

	virtual void Reset() = 0;

	virtual bool Wait(uint32 WaitTime, const bool bIgnoreThreadIdleStats = false) = 0;

	bool Wait()
	{
		return Wait(MAX_uint32);
	}

	//bool Wait(const FTimespan& WaitTime, const bool bIgnoreThreadIdleStats = false)
	//{
	//	return Wait(WaitTime.GetTotalMilliseconds(), bIgnoreThreadIdleStats);
	//}

	FEvent()
		: EventId(0)
		, EventStartCycles(0)
	{}

	virtual ~FEvent()
	{}

	void AdvanceStats();
protected:
	void WaitForStats();

	void TriggerForStats();

	void ResetForStats();

	static uint32 EventUniqueId;

	uint32 EventId;

	uint32 EventStartCycles;
private:
};