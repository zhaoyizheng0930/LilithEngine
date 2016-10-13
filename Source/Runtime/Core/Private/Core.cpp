#include "CorePCH.h"
#include "../Public/Core.h"
//Global Var
bool					GIsRequestingExit = false;

uint32					GGameThreadId = 0;

bool					GbUseHyperthreads = true;

bool					GUseThreadedRendering = true;

bool					GRHISupportsRHIThread = true;

bool					GUseMultithreadDevice = true;

bool					GUseD3DDebug = true;