#pragma once
#include "Param.h"

// 에지 투과 검사
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