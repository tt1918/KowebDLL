#pragma once
#include "Inspect.h"

// Cross Åõ°ú
class InspectCOS1 : Inspect
{
public:
	InspectCOS1();
	~InspectCOS1();

	void Create() override;
	void Release() override;

	void Run() override;
};

