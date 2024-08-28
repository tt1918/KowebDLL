#pragma once
#include "Inspect.h"
class InspectCOSD : public Inspect
{
public:
	InspectCOSD();
	virtual ~InspectCOSD();

	void Create(int width, int height, bool isInit = false) override;
	void Release() override;

	void Run() override;
};

