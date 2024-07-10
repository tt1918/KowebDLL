#pragma once
#include "Inspect.h"
class InspectCOS8 : Inspect
{
public:
	InspectCOS8();
	~InspectCOS8();

	void Create() override;
	void Release() override;

	void Run() override;
};

