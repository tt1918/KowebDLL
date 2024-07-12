#pragma once
#include "Inspect.h"
class InspectCOSD : Inspect
{
public:
	InspectCOSD();
	~InspectCOSD();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

