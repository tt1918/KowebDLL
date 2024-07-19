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

	_pPinhole = &_Pinhole;
	_pCunic = &_Cunic;
	_pPress = &_Press;
}

void Cos2Param::to_json(json& j, const Param& p)
{

}

void Cos2Param::from_json(json& j, const Param& p)
{

}