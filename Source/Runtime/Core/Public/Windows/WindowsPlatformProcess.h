//TODO_SUPPORT windows process multithread later
#pragma once
#include "../GenericPlatform/GenericPlatformProcess.h"
#include "WindowsCriticalSection.h"

class FWindowsPlatformProcess
	: public FGenericPlatformProcess
{
public:

protected:
private:
};

typedef FWindowsPlatformProcess FPlatformProcess;
typedef FWindowsCriticalSection FCriticalSection;