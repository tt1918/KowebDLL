#pragma once
#include "Inspect.h"

// Cross ����
class InspectCOS1 : public Inspect
{
public:
	InspectCOS1();
	virtual ~InspectCOS1();

	void Create(int width, int height, bool isInit=false) override;
	void Release() override;

	void Run() override;
};

