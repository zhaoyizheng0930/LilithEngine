#pragma once
#include "QueuedWork.h"
#include "QueuedThread.h"

class FScopeLock
{
public:

	/**
	* Constructor that performs a lock on the synchronization object
	*
	* @param InSynchObject The synchronization object to manage
	*/
	FScopeLock(FCriticalSection* InSynchObject)
		: SynchObject(InSynchObject)
	{
		SynchObject->Lock();
	}

	/** Destructor that performs a release on the synchronization object. */
	~FScopeLock()
	{
		SynchObject->Unlock();
	}
private:

	/** Default constructor (hidden on purpose). */
	FScopeLock();

	/** Copy constructor( hidden on purpose). */
	FScopeLock(const FScopeLock& InScopeLock);

	/** Assignment operator (hidden on purpose). */
	FScopeLock& operator=(FScopeLock& InScopeLock)
	{
		return *this;
	}

private:

	// Holds the synchronization object to aggregate and scope manage.
	FCriticalSection* SynchObject;
};

class FQueuedThreadPool
{
public:

	virtual bool Create(uint32 InNumQueuedThreads, uint32 StackSize = (32 * 1024), EThreadPriority ThreadPriority = TPri_Normal) = 0;

	virtual void Destroy() = 0;

	virtual void AddQueuedWork(IQueuedWork* InQueuedWork) = 0;

	virtual bool RetractQueuedWork(IQueuedWork* InQueuedWork) = 0;

	virtual IQueuedWork* ReturnToPoolOrGetNextJob(class FQueuedThread* InQueuedThread) = 0;

public:
	virtual ~FQueuedThreadPool() { }

public:
	static FQueuedThreadPool* Allocate();

protected:
private:
};