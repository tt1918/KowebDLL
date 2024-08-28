#pragma once
#include "Inspect.h"
class InspectCOSA : public Inspect
{
public:
	InspectCOSA();
	virtual ~InspectCOSA();

	void Create(int width, int height, bool isInit = false) override;
	void Release() override;

	void Run() override;
};

