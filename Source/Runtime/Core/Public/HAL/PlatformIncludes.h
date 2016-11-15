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
#include <iostream>
#include <sstream>
#include <stack>
#include <sstream>
#include <algorithm>
#include <map>

#ifdef _DEBUG
	#include <assert.h>
#endif

//Generic Platform Include
#include "../GenericPlatform/GenericPlatformMemory.h"
#include "../GenericPlatform/GenericPlatformProcess.h"
#include "../GenericPlatform/GenericApplication.h"
#include "../GenericPlatform/GenericWindow.h"

#ifdef PLATFORM_WINDOWS
	#include "../Windows/WindowsPlatformIncludes.h"
#endif