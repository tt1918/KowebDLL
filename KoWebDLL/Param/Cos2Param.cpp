#include "Cos2Param.h"

Cos2Param::Cos2Param()
{
	ResetAddress();
	SetAddress();
}

Cos2Param::~Cos2Param()
{

}

void Cos2Param::SetParam()
{
	Param::SetParam();

	////////////////////////////////////////////////
	// πÈ¡° 
	_LvUp.AreaTh = (int)_Data[31];
	for (int i = 0; i < 10; i++)
	{
		_LvUp.LvTH[i] = (int)_Data[70 + i];
		_LvUp.LvSize[i] = _Data[80 + i];
	}
	////////////////////////////////////////////////
	
	////////////////////////////////////////////////
	// »Ê¡°
	_LvDn.AreaTh = (int)_Data[32];
	for (int i = 0; i < 10; i++)
	{
		_LvDn.LvTH[i] = (int)_Data[90 + i];
		_LvDn.LvSize[i] = _Data[100 + i];
	}
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	// ¬Ô»˚ ∞ÀªÁ.
	_Press.UpTh = (int)_Data[12];
	_Press.DnTh = (int)_Data[13];
	_Press.AreaUpTH = (int)_Data[14];
	_Press.AreaDnTH = (int)_Data[15];

	for (int i = 0; i < 10; i++)
	{
		_Press.Value[i] = (int)_Data[130+i];
		_Press.Size[i] = _Data[140 + i];
	}
	
	_Press.IsInsp = false;
	for (int i = 0; i < MAX_LEVEL; i++)
	{
		if (_UseLv[i] == true && (_Press.Value[i] > 0 || _Press.Size[i] > 0.001))
		{
			_Press.IsInsp = true;
			break;
		}
	}
	////////////////////////////////////////////////

	
	////////////////////////////////////////////////
	// æÛ∑Ë ∞ÀªÁ.
	_Cunic.InspArea = (int)_Data[10];	// ¿Ãπ∞º∫ ƒÌ¥– ∞ÀªÁ øµø™ 
	_Cunic.CandiVal = (int)_Data[11];	// ¿Ãπ∞º∫ ƒÌ¥– Value

	if (_Cunic.InspArea < 24)		_Cunic.InspArea = 24;
	else if (_Cunic.InspArea > 80)	_Cunic.InspArea = 80;

	for (int i = 0; i < MAX_LEVEL; i++)
	{
		_Cunic.Value1[i]	= _Data[150 + i];
		_Cunic.Std1[i]		= _Data[160 + i];
		_Cunic.Std[i]		= _Data[170 + i];
		if (_Cunic.Std[i] > 100.0) _Cunic.Std[i] /= 1000.0;
	}

	////////////////////////////////////////////////
}

// µ•¿Ã≈Õ 
void Cos2Param::SetInstParam(InspParam::COMMON_PARAM *pCommon, 
	InspParam::CAM_PARAM	*pCam, 
	InspParam::HOLE_SKIP_PARAM *pHoleSkip, 
	bool *pUseLv,
	float *pCycle,
	InspParam::SPOT_PARAM* pLvUp, 
	InspParam::SPOT_PARAM* pLvDn, 
	InspParam::PRESS_PARAM* pPress,
	InspParam::CUNIC_PARAM* pCunic, 
	InspParam::PINHOLE_PARAM* pPinhole, 
	InspParam::SC_PARAM* pSc)
{
	memcpy(&Common, pCommon, sizeof(InspParam::COMMON_PARAM));
	memcpy(&Cam, pCam, sizeof(InspParam::COMMON_PARAM));
	memcpy(&HoleSkip, pHoleSkip, sizeof(InspParam::HOLE_SKIP_PARAM));
	memcpy(&_UseLv, pUseLv, sizeof(bool));
	memcpy(&_Cycle, pCycle, sizeof(float));

	if (pLvUp != nullptr)	memcpy(&_LvUp, pLvUp, sizeof(InspParam::SPOT_PARAM));
	else					_LvUp.Init(); 

	if (pLvDn != nullptr)	memcpy(&_LvDn, pLvDn, sizeof(InspParam::SPOT_PARAM));
	else					_LvDn.Init();

	if (pPress != nullptr)	memcpy(&_Press, pPress, sizeof(InspParam::PRESS_PARAM));
	else					_Press.Init(); 

	if (pCunic != nullptr) 	memcpy(&_Cunic, pCunic, sizeof(InspParam::CUNIC_PARAM));
	else					_Cunic.Init(); 

	if (pPinhole != nullptr) memcpy(&_Pinhole, pPinhole, sizeof(InspParam::PINHOLE_PARAM));
	else					_Pinhole.Init(); 

	if (pSc != nullptr)		memcpy(&_SC, pSc, sizeof(InspParam::SC_PARAM));
	else					_SC.Init();
}

void Cos2Param::GetParam()
{

}

void Cos2Param::Apply()
{

}

void Cos2Param::SetAddress()
{
	_pLvUp = &_LvUp;
	_pLvDn = &_LvDn;

	_pPress = &_Press;

	_pPinhole = &_Pinhole;

	_pCunic = &_Cunic;

	_pSC = &_SC;
}

void Cos2Param::to_json(json& j, const Param& p)
{

}

void Cos2Param::from_json(json& j, const Param& p)
{

}