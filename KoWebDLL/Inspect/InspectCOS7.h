#pragma once
#include "Inspect.h"
class InspectCOS7 : public Inspect
{
public:
	InspectCOS7();
	virtual ~InspectCOS7();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

