#pragma once

//TODO:make it modularization later.

extern void CreateDefaultMainFrame(const bool bStartImmersive, const bool bStartPIE);

class SlateApplication
{
public:
	SlateApplication();

	void MakeWindow();

	static void AddWindow();
protected:
private:
};

extern SlateApplication* SlateApplicationIns;