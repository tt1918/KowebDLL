#pragma once
#include "Inspect.h"
class InspectCOSC : public Inspect
{
public:
	InspectCOSC();
	virtual ~InspectCOSC();

	void Create(int width, int height, bool isInit = false) override;
	void Release() override;

	void Run() override;
};

