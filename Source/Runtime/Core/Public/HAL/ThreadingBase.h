#pragma once
#include "Thread/QueuedThreadPool.h"

extern FQueuedThreadPool* GThreadPool;

/**
* Implementation of a queued thread pool.
*/
class FQueuedThreadPoolBase : public FQueuedThreadPool
{
protected:
	std::vector<IQueuedWork*> QueuedWork;

	std::vector<FQueuedThread*> QueuedThreads;

	std::vector<FQueuedThread*> AllThreads;

	FCriticalSection* SynchQueue;

public:
	FQueuedThreadPoolBase()
		:SynchQueue(nullptr)
	{
		
	}

	virtual ~FQueuedThreadPoolBase()
	{
		Destroy();
	}

	virtual bool Create(uint32 InNumQueuedThreads, uint32 StackSize = (32 * 1024), EThreadPriority ThreadPriority = TPri_Normal) override
	{
		SynchQueue = new FCriticalSection();
		FScopeLock Lock(SynchQueue);

		QueuedThreads.clear();
		bool bFailse = false;
		for (int i = 0; i < InNumQueuedThreads;i++)
		{
			FQueuedThread* pThread = new FQueuedThread();
			if (pThread->Create(this , StackSize , ThreadPriority) == true)
			{
				QueuedThreads.push_back(pThread);
				AllThreads.push_back(pThread);
			}
			else
			{
				bFailse = true;
				delete pThread;
			}
		}

		if (bFailse)
		{
			Destroy();
		}

		return !bFailse;
	}

	virtual void Destroy() override
	{
		if (SynchQueue)
		{
			//Abandon All ThreadWork;
			{
				FScopeLock Lock(SynchQueue);
				for (int i = 0; i < QueuedWork.size(); i++)
				{
					QueuedWork[i]->Abandon();
				}
				QueuedWork.clear(); 
			}

			//wait all thread finish
			while (1)
			{
				{
					FScopeLock Lock(SynchQueue);
					if (AllThreads.size() == QueuedThreads.size())
					{
						break;
					}
				}
				FPlatformProcess::Sleep(0.0f);
			}

			//Clear All thread
			{
				FScopeLock Lock(SynchQueue);
				for (int i = 0; i < AllThreads.size(); i++)
				{
					AllThreads[i]->KillThread();
					delete AllThreads[i];
				}
				AllThreads.clear();
				QueuedThreads.clear();
			}

			//delete syncQueue
			delete SynchQueue;
			SynchQueue = nullptr;
		}
	}

	int32 GetNumQueuedJobs()
	{
		return QueuedWork.size();
	}

	void AddQueuedWork(IQueuedWork* InQueuedWork) override
	{
		if (SynchQueue)
		{
			FScopeLock Lock(SynchQueue);
			//GetFist ThreadCanUsed
			FQueuedThread* pThread = nullptr;
			if (QueuedThreads.size())
			{
				std::vector<FQueuedThread*>::iterator iter = QueuedThreads.begin();
				pThread = *iter;
				QueuedThreads.erase(iter);
			}

			//add work
			if (pThread)
			{
				pThread->DoWork(InQueuedWork);
			}
			else
			{
				//no thread todo
				QueuedWork.push_back(InQueuedWork);
			}
		}
	}

	virtual bool RetractQueuedWork(IQueuedWork* InQueuedWork) override
	{
		FScopeLock Lock(SynchQueue);

		std::vector<IQueuedWork*>::iterator iter = find(QueuedWork.begin(),QueuedWork.end(), InQueuedWork);
		if (iter != QueuedWork.end())
		{
			delete *iter;
			QueuedWork.erase(iter);
		}
		return true;
	}

	virtual IQueuedWork* ReturnToPoolOrGetNextJob(FQueuedThread* InQueuedThread) override
	{
		FScopeLock Lock(SynchQueue);
		IQueuedWork* work = nullptr;
		if (QueuedWork.size() > 0)
		{
			std::vector<IQueuedWork*>::iterator iter = QueuedWork.begin();
			work = *iter;
			QueuedWork.erase(iter);
		}
		if (work == nullptr)
		{
			QueuedThreads.push_back(InQueuedThread);
		}
		return work;
	}
};

