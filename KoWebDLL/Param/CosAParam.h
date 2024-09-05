#pragma once
#include "Param.h"

// 
class CosAParam : public Param
{
public:
	InspParam::PINHOLE_PARAM	Pinhole;		// Hole �˻� �Ķ����
	InspParam::GROUP_PARAM		Group;			// ���� ���� �Ķ����

	int							JudgeFishEye;	// �Ǻҷ� ó�� ����

	InspParam::CUNIC_PARAM		Cunic;			// ��� ���� �Ķ����

	int							SCValue;		// ��ũ��ġ ���Ⱚ �Ķ����

	InspParam::LONG_SC_PARAM	LongSC;			// Long SC �Ķ����

public:
	CosAParam();
	~CosAParam();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;

public:

};