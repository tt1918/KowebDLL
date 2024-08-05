#pragma once
#include "Inspect.h"

// Edge Transfer Inspect
class InspectCOS2 : public  Inspect
{
public:
	InspectCOS2();
	virtual ~InspectCOS2();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

