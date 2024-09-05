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
	// ���� ����
	double				_Data[300];

	// �Ķ���� Ÿ�� ó��
	eParamType::eType	_ParamType;		
	std::wstring		_MachineName;
	std::wstring		_ModelName;

public:
	// ���� �Ķ���� ���� ����
	// ������
	InspParam::BASIC_INSP_PARAM OneUp;
	// ������
	InspParam::BASIC_INSP_PARAM OneDn;

	// ���� �Ķ����
	InspParam::COMMON_PARAM		Common;
	InspParam::CAM_PARAM		Cam;
	InspParam::HOLE_SKIP_PARAM	HoleSkip;

	bool						_UseLv[MAX_LEVEL];

	float						_Cycle[MAX_CYCLE];

	// �Ǻҷ� ���� ����
	int							*_pCheckFishEyeArea;

	// ��Ȧ ���� �Ķ����
	InspParam::PINHOLE_PARAM	*_pPinhole;
	
	// �׸� ���� �ٲ�� �Ķ���ʹ� �������� ����
	// Level �Ķ����
	InspParam::SPOT_PARAM		*_pLvUp;
	InspParam::SPOT_PARAM		*_pLvDn;

	// ���� �ҷ� �Ķ����
	InspParam::GROUP_PARAM		*_pGroup;

	// ��� �ҷ� ���� �Ķ���� 
	InspParam::CUNIC_PARAM		*_pCunic;

	// SC �Ķ����
	InspParam::SC_PARAM			*_pSC;

	// Long SC �Ķ����
	InspParam::LONG_SC_PARAM	*_pLongSC;

	// ���� �Ķ����
	InspParam::PRESS_PARAM		*_pPress;

	// BCR �ν� �Ķ����
	InspParam::BCR_PARAM		*_pBCR;

	// ���� �̹� �Ķ����
	InspParam::KIPO_PARAM		*_pKipo;

public:
	Param();
	virtual ~Param();

	void InitParam();

	virtual void SetParam(); // �Ķ���� ���� ó��
	virtual void GetParam();
	virtual void Apply();							

	virtual void SetAddress();
	void ResetAddress();

	void Save(TempParam::PC_INFO* pcInfo, std::wstring path, SystemParam* pSysParam);			// ���� �����͸� ����
	bool Load(TempParam::PC_INFO* pcInfo, std::wstring path, SystemParam* pSysParam);			// ���Ͽ��� ������ �о����

	// Algorithm Ȯ��
	static eParamType::eType GetParamType(std::wstring path, std::wstring section);

public:

	//�Ķ���� ������ �˷��ش�.
	eParamType::eType GetParamType() { return _ParamType; }
};