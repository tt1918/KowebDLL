#pragma once
#include "Inspect.h"
class InspectCOS5 : Inspect
{
public:
	InspectCOS5();
	~InspectCOS5();

	void Create() override;
	void Release() override;

	void Run() override;
};

