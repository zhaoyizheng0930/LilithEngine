#include "RenderCorePCH.h"
#include "../Public/RenderingThread.h"
#include "HAL/Thread/Runnable.h"

//Whether the renderer is currently running in a separate thread.
bool GIsThreadedRendering = false;

//bool GUseThreadedRendering = false;

//RHI Thread and RenderingThread are different;
bool GUseRHIThread = false;


static FRunnable* GRenderingThreadRunnable = nullptr;

static FRunnableThread* GRenderingThread = nullptr;

void RenderingThreadMain()
{

}

class FRenderThread:public FRunnable
{
public:
	virtual bool Init()
	{
		GRenderThreadId = FPlatformTLS::GetCurrentThreadId();
	}
	virtual uint32 Run()
	{
		//FMemory::SetupTLSCachesOnCurrentThread();

		//FPlatformProcess::SetupRenderThread();

		RenderingThreadMain();

		//FMemory::ClearAndDisableTLSCachesOnCurrentThread();
		
		return 0;
	}

	virtual void Stop()
	{ 

	}

	virtual void Exit() 
	{
		GRenderThreadId = 0;
	}
protected:
private:
};

void StartRenderingThread()
{
	GIsThreadedRendering = true;

	//CreateRenderingThread
	GRenderingThreadRunnable = new FRenderThread();

	GRenderingThread = FRunnableThread::Create(GRenderingThreadRunnable, "RenderingThread", 0, TPri_Normal, FGenericPlatformAffinity::GetRenderingThreadMask());

	//Create RenderThread Heart Beat Listener Thread Later
}

void StopRenderingThread()
{

}