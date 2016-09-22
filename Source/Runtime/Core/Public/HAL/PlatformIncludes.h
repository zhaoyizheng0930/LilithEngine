#pragma once

#include <new>
#include <wchar.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>
#include <string.h>

#include <string>
#include <vector>

//Generic Platform Include
#include "../GenericPlatform/GenericPlatformMemory.h"

#if PLATFORM_WINDOWS
	#include "../Windows/WindowsPlatformIncludes.h"
#elif PLATFORM_PS4
#endif