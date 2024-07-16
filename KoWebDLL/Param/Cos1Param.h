#pragma once
#include "Param.h"

// 
class Cos1Param : Param
{
private:
	InspParam::PINHOLE_PARAM	Pinhole;		// Hole �˻� �Ķ����
	InspParam::GROUP_PARAM		Group;			// ���� ���� �Ķ����
	int							JudgeFishEye;	// �Ǻҷ� ó�� ����

	InspParam::CUNIC_PARAM		Cunic;			// ��� ���� �Ķ����

	InspParam::SC_PARAM			SC;				// ��ũ��ġ ���Ⱚ �Ķ����
	InspParam::LONG_SC_PARAM	LongSC;			// Long SC �Ķ����

public:
	Cos1Param();
	~Cos1Param();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;

	void to_json(json& j, const Param& p);
	void from_json(json& j, const Param& p);

public:

};