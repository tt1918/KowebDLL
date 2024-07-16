#pragma once
#include "Param.h"

// 
class CosEParam : Param
{
public:
	InspParam::PINHOLE_PARAM	Pinhole;		// Hole 검사 파라미터
	InspParam::GROUP_PARAM		Group;			// 군집 판정 파라미터

	int							JudgeFishEye;	// 판불량 처리 여부

	InspParam::CUNIC_PARAM		Cunic;			// 쿠닉 판정 파라미터

	int							SCValue;		// 스크라치 검출값 파라미터

	InspParam::LONG_SC_PARAM	LongSC;			// Long SC 파라미터

public:
	CosEParam();
	~CosEParam();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;

	void to_json(json& j, const Param& p);
	void from_json(json& j, const Param& p);

public:

};