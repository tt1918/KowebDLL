#include "Cos1Param.h"

Cos1Param::Cos1Param()
{
	SetAddress();
}

Cos1Param::~Cos1Param()
{

}

void Cos1Param::SetParam()
{

}

void Cos1Param::GetParam()
{

}

void Cos1Param::Apply()
{

}

// �Ķ���Ϳ� ���� ������ ����
void Cos1Param::SetAddress()
{
	_pPinhole = &Pinhole;
	_pGroup = &Group;
	_pCheckFishEyeArea = &JudgeFishEye;
	_pCunic = &Cunic;
	_pSC = &SC;
	_pLongSC = &LongSC;
}