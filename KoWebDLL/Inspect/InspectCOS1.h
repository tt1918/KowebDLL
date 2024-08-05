#pragma once
#include "Inspect.h"

// Cross Åõ°ú
class InspectCOS1 : public Inspect
{
public:
	InspectCOS1();
	virtual ~InspectCOS1();

	void Create(int width, int height) override;
	void Release() override;

	void Run() override;
};

