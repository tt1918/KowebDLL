#pragma once
#include "Inspect.h"
class InspectCOS9 : public Inspect
{
public:
	InspectCOS9();
	virtual ~InspectCOS9();

	void Create(int width, int height, bool isInit = false) override;
	void Release() override;

	void Run() override;
};

