#pragma once
#include "Inspect.h"
class InspectCOSE : public Inspect
{
public:
	InspectCOSE();
	~InspectCOSE();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

