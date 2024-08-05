#pragma once
#include "Inspect.h"

// Full Refect Inspect
class InspectCOS3 : public  Inspect
{
public:
	InspectCOS3();
	virtual ~InspectCOS3();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

