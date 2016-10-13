#pragma once

#include "DynamicRHI.h"

/** Initializes the RHI. */
extern void RHIInit(bool bHasEditorToken);

/** Performs additional RHI initialization before the render thread starts. */
extern void RHIPostInit();

/** Shuts down the RHI. */
extern void RHIExit();

extern FDynamicRHI* CreateRHI();