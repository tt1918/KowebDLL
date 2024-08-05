#include "Cos1Param.h"

Cos1Param::Cos1Param()
{
	ResetAddress();

	SetAddress();
}

Cos1Param::~Cos1Param()
{

}

void Cos1Param::SetParam()
{
	Param::SetParam();
}

void Cos1Param::GetParam()
{

}

void Cos1Param::Apply()
{

}

// 파라미터와 상위 포인터 연결
void Cos1Param::SetAddress()
{
	_pPinhole = &Pinhole;
	_pGroup = &Group;
	_pCheckFishEyeArea = &JudgeFishEye;
	_pCunic = &Cunic;
	_pSC = &SC;
	_pLongSC = &LongSC;
}

void Cos1Param::to_json(json& j, const Param& p)
{

}

void Cos1Param::from_json(json& j, const Param& p)
{

}