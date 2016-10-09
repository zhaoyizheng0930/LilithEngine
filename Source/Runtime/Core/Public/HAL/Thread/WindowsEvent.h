#pragma once
#include "Event.h"
#include "../PlatformIncludes.h"

class FEventWin:public FEvent
{
public:
	FEventWin() {}
	~FEventWin() {}

	virtual bool Create(bool bIsManualReset = false)
	{
		Event = CreateEvent(nullptr , bIsManualReset , 0 , nullptr);
		ManualReset = bIsManualReset;
		return Event != nullptr;
	}

	virtual bool IsManualReset()
	{
		return ManualReset;
	}

	virtual void Trigger()
	{
		TriggerForStats();
		SetEvent(Event);
	}

	virtual void Reset()
	{
		ResetForStats();
		ResetEvent(Event);
	}

	virtual bool Wait(uint32 WaitTime, const bool bIgnoreThreadIdleStats = false)
	{
		WaitForStats();
		return (WaitForSingleObject(Event, WaitTime) == WAIT_OBJECT_0);
	}

protected:
private:

	/** Holds the handle to the event. */
	HANDLE Event;

	/** Whether the signaled state of the event needs to be reset manually. */
	bool ManualReset;
};