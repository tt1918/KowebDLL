#pragma once
#include "Inspect.h"
class InspectCOSBCR : public  Inspect
{
public:
	InspectCOSBCR();
	virtual ~InspectCOSBCR();

	void Create(int width, int height, bool isInit = false) override;
	void Release() override;

	void Run() override;
};

