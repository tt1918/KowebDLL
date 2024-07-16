#pragma once
#include "Param.h"

// S1 �˻縦 ����
class Cos4Param : Param
{
public:
	InspParam::PINHOLE_PARAM	Pinhole;		// Hole �˻� �Ķ����
	InspParam::GROUP_PARAM		Group;			// ���� ���� �Ķ����

	int							JudgeFishEye;	// �Ǻҷ� ó�� ����

	InspParam::CUNIC_PARAM		Cunic;			// ��� ���� �Ķ����

	int							SCValue;		// ��ũ��ġ ���Ⱚ �Ķ����

	InspParam::LONG_SC_PARAM	LongSC;			// Long SC �Ķ����

public:
	Cos4Param();
	~Cos4Param();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;

	void to_json(json& j, const Param& p);
	void from_json(json& j, const Param& p);

public:

};