#include "EnginePCH.h"
#include "Shader/ShaderCompiler.h"
#include "HAL/Thread/Runnable.h"

//Unreal's Compiler Thread support later.
class FShaderCompileThreadRunnableBase:public FRunnable
{
public:
	virtual uint32 Run()
	{

	}
protected:
private:
};


class FShaderCompilingManager
{
public:
	//void CompileAllShader();
protected:
private:
};