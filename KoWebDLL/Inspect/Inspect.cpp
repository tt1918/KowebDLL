#include "Inspect.h"

Inspect::Inspect()
{
	Init();
}


Inspect::~Inspect()
{
}

void Inspect::Init()
{
	_pParam = nullptr;
	_pFlatImg = nullptr;
	_pProfile = nullptr;
	_pEdgeFinder = nullptr;
}

void Inspect::Create()
{

}

void Inspect::Release()
{

}

void Inspect::Run()
{

}

void Inspect::SpotInspect()
{
	if (_pParam == nullptr) return;

	int i, j, nOverlap;
	int nLevel, nLevel1, nAvg, nValueUp, nValueDn, nAvg1, nValueUp1, nValueDn1, nValueUpBig, nValueDnBig;
	double dSize[5], dSize1[5], dSizeBig[5];
	int nPLevel, nPValueUp, nPValueDn, nPAvg;	//찍힘검사
	int nClass, nClass_;
	double dPSize[5], dImulKipoSize, dImulKipoSize1, dImulKipoSizeBig;
	int nWhiteInspect = 0, nBlackInspect = 0, nCunicInspect = 0;
	int nSkip, leftBig, topBig, nDefectPosX, nDefectPosY, nDefectBigPosX, nDefectBigPosY;
	double dCunic;
	int nCunicValue;
	int nFoundLami = 0;
	int nOverkill = 0, nBigDefect;

	_pTmpParam->MaxGray = 0;
	_pTmpParam->MinGray = 0;

	int nArThUp = _pParam->Common.AreaUpTh;
	int nArThDn = _pParam->Common.AreaDnTh;

	InspParam::SPOT_PARAM* pLvUp = _pParam->_pLvUp;
	InspParam::SPOT_PARAM* pLvDn = _pParam->_pLvDn;
	InspParam::CUNIC_PARAM* pCunic = _pParam->_pCunic;
	InspParam::PRESS_PARAM* pPress = _pParam->_pPress;

	// 백/흑/찍힘 파라미터 체크
	for (i = 0; i < 10; i++)
	{
		if (_pParam->UseLv[i] == true)
		{
			if (pLvUp != nullptr && (pLvUp->Level[i].TH > 0 || pLvUp->Level[i].Size > 0.0))	nWhiteInspect = 1;
			if (pLvDn != nullptr && (pLvDn->Level[i].TH > 0 || pLvDn->Level[i].Size > 0.0))	nBlackInspect = 1;
			if (pCunic != nullptr && (pCunic->Std[i] > 0.0 || pCunic->Std1[i] > 0.0)) nCunicInspect = 1;
		}
	}

	// 찍힘 검사 파라미터 체크 
	if (pPress != nullptr)
	{
		for (i = 0; i < 10; i++)
		{
			if (_pParam->UseLv[i] == true && (pPress->Value[i] > 0 || pPress->Size[i] > 0.001))
			{
				nWhiteInspect = nBlackInspect = 1;
				break;
			}
		}
	}

	// 조명 제어 중에는 쿠닉 검사를 하지 않는다. 
	if (_pTmpParam->AutoAdjStart > 0) nCunicInspect = 0;

	// 피라미드 영상을 생성
	// 
	if (_pTmpParam->MakePyramidDone == 0)
	{

	}


}