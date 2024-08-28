#pragma once
#include "Param.h"

// 에지 투과 검사
class Cos2Param : public Param
{
public:
	InspParam::PINHOLE_PARAM	_Pinhole;
	InspParam::CUNIC_PARAM		_Cunic;		// 쿠닉 검사이나 여기서는 얼룩으로 대체
	InspParam::PRESS_PARAM		_Press;
	InspParam::SPOT_PARAM		_LvUp;
	InspParam::SPOT_PARAM		_LvDn;
	InspParam::SC_PARAM			_SC;

public:
	Cos2Param();
	~Cos2Param();

	void SetParam() override;
	void SetInstParam(InspParam::COMMON_PARAM* pCommon,
		InspParam::CAM_PARAM* pCam,
		InspParam::HOLE_SKIP_PARAM* pHoleSkip,
		bool* pUseLv,
		float* pCycle,
		InspParam::SPOT_PARAM* pLvUp,
		InspParam::SPOT_PARAM* pLvDn,
		InspParam::PRESS_PARAM* pPress,
		InspParam::CUNIC_PARAM* pCunic,
		InspParam::PINHOLE_PARAM* pPinhole,
		InspParam::SC_PARAM* pSc);

	void GetParam() override;
	void Apply() override;

	void SetAddress() override;


	void to_json(json& j, const Param& p);
	void from_json(json& j, const Param& p);

public:

};