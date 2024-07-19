#pragma once
#include "Inspect.h"
class InspectCOSC : public Inspect
{
public:
	InspectCOSC();
	~InspectCOSC();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

