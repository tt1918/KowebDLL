#pragma once
#include "Inspect.h"

// S1(45 Degree Edge Transfer) Inspect
class InspectCOS4 : public Inspect
{
public:
	InspectCOS4();
	~InspectCOS4();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

