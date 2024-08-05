#pragma once
#include "Inspect.h"
class InspectCOS6 : public Inspect
{
public:
	InspectCOS6();
	virtual ~InspectCOS6();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

