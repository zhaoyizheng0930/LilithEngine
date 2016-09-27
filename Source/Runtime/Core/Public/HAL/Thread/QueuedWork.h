#pragma once

class IQueuedWork
{
public:
	virtual void DoThreadedWork() = 0;

	virtual void Abandon() = 0;

public:
	virtual ~IQueuedWork() { }
protected:
private:
};

typedef IQueuedWork FQueuedWork;