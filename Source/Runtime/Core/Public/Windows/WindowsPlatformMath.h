#pragma once
#include "GenericPlatform/GenericPlatformMath.h"

//Temp Use XNA math lib.
//Do myself math lib later.

#include "Windows/XNA/xnamath.h"
//
typedef XMFLOAT3 FVector3;
typedef XMFLOAT2 FVector2;
typedef XMMATRIX FMatrix;

class FWindowsPlatformMath :public FGenericPlatformMath
{
public:
protected:
private:
};

typedef FWindowsPlatformMath FPlatformMath;