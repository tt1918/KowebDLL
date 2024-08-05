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
	// ¹éÁ¡ 
	_LvUp.AreaTh = (int)_Data[31];
	for (int i = 0; i < 10; i++)
	{
		_LvUp.LvTH[i] = (int)_Data[70 + i];
		_LvUp.LvSize[i] = _Data[80 + i];
	}
	////////////////////////////////////////////////
	
	////////////////////////////////////////////////
	// ÈæÁ¡
	_LvDn.AreaTh = (int)_Data[32];
	for (int i = 0; i < 10; i++)
	{
		_LvDn.LvTH[i] = (int)_Data[90 + i];
		_LvDn.LvSize[i] = _Data[100 + i];
	}
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	// ÂïÈû °Ë»ç.
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
	// ¾ó·è °Ë»ç.
	_Cunic.InspArea = _Data[10];	// ÀÌ¹°¼º Äí´Ð °Ë»ç ¿µ¿ª 
	_Cunic.CandiVal = _Data[11];	// ÀÌ¹°¼º Äí´Ð Value

	if (_Cunic.InspArea < 24) _Cunic.InspArea = 24;
	else if (_Cunic.InspArea > 80) _Cunic.InspArea = 80;

	for (int i = 0; i < MAX_LEVEL; i++)
	{
		_Cunic.Value1[i] = _Data[150 + i];
		_Cunic.Std1[i] = _Data[160 + i];
		_Cunic.Std[i] = _Data[170 + i];
		if (_Cunic.Std[i] > 100.0) _Cunic.Std[i] /= 1000.0;
	}

	////////////////////////////////////////////////
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
}

void Cos2Param::to_json(json& j, const Param& p)
{

}

void Cos2Param::from_json(json& j, const Param& p)
{

}