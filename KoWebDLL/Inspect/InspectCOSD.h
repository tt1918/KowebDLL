#pragma once
#include "Inspect.h"
class InspectCOSD : Inspect
{
public:
	InspectCOSD();
	~InspectCOSD();

	void Create() override;
	void Release() override;

	void Run() override;
};

