#pragma once
#include "Inspect.h"

// S1(45 Degree Edge Transfer) Inspect
class InspectCOS4 : public Inspect
{
public:
	InspectCOS4();
	virtual ~InspectCOS4();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

