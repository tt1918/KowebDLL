#pragma once
#include "Inspect.h"
class InspectCOSBCR : Inspect
{
public:
	InspectCOSBCR();
	~InspectCOSBCR();

	void Create() override;
	void Release() override;

	void Run() override;
};

