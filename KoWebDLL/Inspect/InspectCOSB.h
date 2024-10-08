#pragma once
#include "Inspect.h"
class InspectCOSB : public Inspect
{
public:
	InspectCOSB();
	virtual ~InspectCOSB();

	void Create(int width, int height, bool isInit = false) override;
	void Release() override;

	void Run() override;
};

