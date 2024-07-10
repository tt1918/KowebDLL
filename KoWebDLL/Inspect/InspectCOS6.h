#pragma once
#include "Inspect.h"
class InspectCOS6 : Inspect
{
public:
	InspectCOS6();
	~InspectCOS6();

	void Create() override;
	void Release() override;

	void Run() override;
};

