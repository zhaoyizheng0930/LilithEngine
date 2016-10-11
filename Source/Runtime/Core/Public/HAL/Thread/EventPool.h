#pragma once
#include "Event.h"

enum class EEventPoolTypes
{
	AutoReset,

	ManualReset
};

template<EEventPoolTypes PoolType>
class FEventPool
{
public:
	static FEventPool& Get()
	{
		static FEventPool Singleton;
		return Singleton;
	}

	FEvent* GetEventFromPool()
	{
		FEvent* Result = nullptr;
		if (Pool.size() > 0)
		{
			Result = Pool.top();
			Pool.pop();
		}
		else
		{
			Result = FPlatformProcess::CreateSynchEvent();
		}

		return Result;
	}

	void ReturnToPool(FEvent* Event)
	{
		Event->Reset();
		Pool.push(Event);
	}
protected:
private:
	std::stack<FEvent*> Pool;
	//std::vector<FEvent*> Pool;
};