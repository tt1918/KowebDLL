#pragma once
#include "Param.h"

// ���� ���� �˻�
class Cos2Param : Param
{
public:
	InspParam::PRESS_PARAM Press;

public:
	Cos2Param();
	~Cos2Param();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;
public:

};