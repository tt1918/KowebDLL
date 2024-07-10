#pragma once
#include "Param.h"

// 
class Cos7Param : Param
{
public:
	InspParam::PINHOLE_PARAM	Pinhole;		// Hole �˻� �Ķ����
	InspParam::GROUP_PARAM		Group;			// ���� ���� �Ķ����

	int							JudgeFishEye;	// �Ǻҷ� ó�� ����

	InspParam::CUNIC_PARAM		Cunic;			// ��� ���� �Ķ����

	int							SCValue;		// ��ũ��ġ ���Ⱚ �Ķ����

	InspParam::LONG_SC_PARAM	LongSC;			// Long SC �Ķ����

public:
	Cos7Param();
	~Cos7Param();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;
public:

};