#include "Param.h"

Param::Param()
{
	ResetAddress();
}

Param::~Param()
{

}

void Param::SetParam()
{

}

void Param::GetParam()
{

}

void Param::Apply()
{

}

void Param::SetAddress()
{

}

void Param::ResetAddress()
{
	_pCheckFishEyeArea = nullptr;
	_pPinhole = nullptr;
	_pGroup = nullptr;
	_pLvUp = nullptr;
	_pLvDn = nullptr;
	_pCunic = nullptr;
	_pSC = nullptr;
	_pLongSC = nullptr;
	_pPress = nullptr;
	_pBCR = nullptr;
	_pKipo = nullptr;
}

void Param::Save(std::string path)
{

}

bool Param::Load(std::string path)
{
	bool isRes = true;
	SetParam();

	return isRes;
}