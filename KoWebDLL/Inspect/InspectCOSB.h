#pragma once
#include "Inspect.h"
class InspectCOSB : Inspect
{
public:
	InspectCOSB();
	~InspectCOSB();

	void Create() override;
	void Release() override;

	void Run() override;
};

