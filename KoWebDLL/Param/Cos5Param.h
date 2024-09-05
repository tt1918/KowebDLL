#pragma once
#include "Param.h"

// 미세기포 군집 검사를 진행
class Cos5Param : public Param
{
public:
	InspParam::GROUP_PARAM		Group;			// 군집 판정 파라미터

public:
	Cos5Param();
	~Cos5Param();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;

public:

};