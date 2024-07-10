#pragma once
#include "Inspect.h"
class InspectCOS9 : Inspect
{
public:
	InspectCOS9();
	~InspectCOS9();

	void Create() override;
	void Release() override;

	void Run() override;
};

