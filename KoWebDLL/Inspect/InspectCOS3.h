#pragma once
#include "Inspect.h"

// Full Refect Inspect
class InspectCOS3 : Inspect
{
public:
	InspectCOS3();
	~InspectCOS3();

	void Create() override;
	void Release() override;

	void Run() override;
};

