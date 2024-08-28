#pragma once
#include "Inspect.h"
class InspectCOS5 : public Inspect
{
public:
	InspectCOS5();
	virtual ~InspectCOS5();

	void Create(int width, int height, bool isInit = false) override;
	void Release() override;

	void Run() override;
};

