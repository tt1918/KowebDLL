#pragma once
#include "Inspect.h"
class InspectCOSA : Inspect
{
public:
	InspectCOSA();
	~InspectCOSA();

	void Create() override;
	void Release() override;

	void Run() override;
};

