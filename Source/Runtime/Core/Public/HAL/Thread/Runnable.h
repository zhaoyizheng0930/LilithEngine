#pragma once

class FRunnable
{
public:
	virtual bool Init()
	{
		return true;
	}

	virtual uint32 Run() = 0;

	virtual void Stop() { }

	virtual void Exit() { }

	virtual class FSingleThreadRunnable* GetSingleThreadInterface()
	{
		return nullptr;
	}

	/** Virtual destructor */
	virtual ~FRunnable() { }
};