#pragma once
#include "enumParam.h"
#include "TempParam.h"
#include "InspParam.h"
#include "../Defines.h"
#include "TempParam.h"
#include "SystemParam.h"

class Param
{
protected:
	// 저장 버퍼
	double				_Data[300];

	// 파라미터 타입 처리
	eParamType::eType	_ParamType;		
	std::wstring		_MachineName;
	std::wstring		_ModelName;

public:
	// 공통 파라미터 정적 생성
	// 강백점
	InspParam::BASIC_INSP_PARAM OneUp;
	// 강흑점
	InspParam::BASIC_INSP_PARAM OneDn;

	// 공통 파라미터
	InspParam::COMMON_PARAM		Common;
	InspParam::CAM_PARAM		Cam;
	InspParam::HOLE_SKIP_PARAM	HoleSkip;

	bool						_UseLv[MAX_LEVEL];

	float						_Cycle[MAX_CYCLE];

	// 판불량 검출 영역
	int							*_pCheckFishEyeArea;

	// 핀홀 검출 파라미터
	InspParam::PINHOLE_PARAM	*_pPinhole;
	
	// 항목에 따라 바뀌는 파라미터는 동적으로 생성
	// Level 파라미터
	InspParam::SPOT_PARAM		*_pLvUp;
	InspParam::SPOT_PARAM		*_pLvDn;

	// 군집 불량 파라미터
	InspParam::GROUP_PARAM		*_pGroup;

	// 쿠닉 불량 검출 파라미터 
	InspParam::CUNIC_PARAM		*_pCunic;

	// SC 파라미터
	InspParam::SC_PARAM			*_pSC;

	// Long SC 파라미터
	InspParam::LONG_SC_PARAM	*_pLongSC;

	// 찍힘 파라미터
	InspParam::PRESS_PARAM		*_pPress;

	// BCR 인식 파라미터
	InspParam::BCR_PARAM		*_pBCR;

	// 기포 이물 파라미터
	InspParam::KIPO_PARAM		*_pKipo;

public:
	Param();
	virtual ~Param();

	void InitParam();

	virtual void SetParam(); // 파라미터 개별 처리
	virtual void GetParam();
	virtual void Apply();							

	virtual void SetAddress();
	void ResetAddress();

	void Save(TempParam::PC_INFO* pcInfo, std::wstring path, SystemParam* pSysParam);			// 현재 데이터를 저장
	bool Load(TempParam::PC_INFO* pcInfo, std::wstring path, SystemParam* pSysParam);			// 파일에서 데이터 읽어오기

	// Algorithm 확인
	static eParamType::eType GetParamType(std::wstring path, std::wstring section);

public:

	//파라미터 형식을 알려준다.
	eParamType::eType GetParamType() { return _ParamType; }
};