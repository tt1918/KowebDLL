#pragma once
#include "Inspect.h"
class InspectCOSC : Inspect
{
public:
	InspectCOSC();
	~InspectCOSC();

	void Create() override;
	void Release() override;

	void Run() override;
};

