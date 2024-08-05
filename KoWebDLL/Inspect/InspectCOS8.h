#pragma once
#include "Inspect.h"
class InspectCOS8 : public Inspect
{
public:
	InspectCOS8();
	virtual ~InspectCOS8();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

