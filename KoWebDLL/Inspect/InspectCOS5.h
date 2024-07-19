#pragma once
#include "Inspect.h"
class InspectCOS5 : public Inspect
{
public:
	InspectCOS5();
	~InspectCOS5();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

