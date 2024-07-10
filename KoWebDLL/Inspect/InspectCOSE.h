#pragma once
#include "Inspect.h"
class InspectCOSE : Inspect
{
public:
	InspectCOSE();
	~InspectCOSE();

	void Create() override;
	void Release() override;

	void Run() override;
};

