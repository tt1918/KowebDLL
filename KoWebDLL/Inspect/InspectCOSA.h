#pragma once
#include "Inspect.h"
class InspectCOSA : public Inspect
{
public:
	InspectCOSA();
	~InspectCOSA();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

