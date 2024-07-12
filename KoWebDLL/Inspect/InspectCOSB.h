#pragma once
#include "Inspect.h"
class InspectCOSB : Inspect
{
public:
	InspectCOSB();
	~InspectCOSB();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

