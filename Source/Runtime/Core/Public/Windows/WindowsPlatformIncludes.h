#pragma once

#include <windows.h>
#include <process.h>

#include "WindowsPlatformMemory.h"
#include "WindowsApplication.h"
#include "WindowsCriticalSection.h"
#include "WindowsPlatformProcess.h"
#include "WindowsPlatformMath.h"

//RELEASE COM OBJ
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }