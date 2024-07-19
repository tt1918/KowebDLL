#pragma once
#include "Inspect.h"
class InspectCOS9 : public Inspect
{
public:
	InspectCOS9();
	~InspectCOS9();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

