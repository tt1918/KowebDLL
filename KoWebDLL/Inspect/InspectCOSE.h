#pragma once
#include "Inspect.h"
class InspectCOSE : public Inspect
{
public:
	InspectCOSE();
	virtual ~InspectCOSE();

	void Create(int width, int height, bool isInit = false) override;
	void Release() override;

	void Run() override;
};

