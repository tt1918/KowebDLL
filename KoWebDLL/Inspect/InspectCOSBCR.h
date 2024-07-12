#pragma once
#include "Inspect.h"
class InspectCOSBCR : Inspect
{
public:
	InspectCOSBCR();
	~InspectCOSBCR();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

