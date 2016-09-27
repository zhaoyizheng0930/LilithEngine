#pragma once

class FSingleThreadRunnable
{
public:

	virtual ~FSingleThreadRunnable() { }

	virtual void Tick() = 0;
};