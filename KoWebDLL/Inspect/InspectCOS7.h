#pragma once
#include "Inspect.h"
class InspectCOS7 : Inspect
{
public:
	InspectCOS7();
	~InspectCOS7();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

