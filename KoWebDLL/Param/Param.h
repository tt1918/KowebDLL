#pragma once
#include "enumParam.h"
#include "TempParam.h"
#include "InspParam.h"
#include "../Defines.h"

class Param
{
protected:
	double			Data[300];
	// �Ķ���� Ÿ�� ó��
	eParamType::eType m_nParamType;		

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

	bool						UseLv[MAX_LEVEL];

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

	virtual void SetParam(); // �Ķ���� ���� ó��
	virtual void GetParam();
	virtual void Apply();							

	virtual void SetAddress();
	void ResetAddress();

	void Save(std::string path);			// ���� �����͸� ����
	bool Load(std::string path);			// ���Ͽ��� ������ �о����
public:

	//�Ķ���� ������ �˷��ش�.
	eParamType::eType GetParamType() { return m_nParamType; }
};