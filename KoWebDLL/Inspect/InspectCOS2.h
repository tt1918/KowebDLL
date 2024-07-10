#pragma once
#include "Inspect.h"

// Edge Transfer Inspect
class InspectCOS2 : Inspect
{
public:
	InspectCOS2();
	~InspectCOS2();

	void Create() override;
	void Release() override;

	void Run() override;
};

