#include "DefectType.h"

DefectType g_NGType;

DefectType::DefectType()
{
	_SpotW = 1;
	_SpotB = 2;
	_SpotPress = 3;
	_Cunic = 4;
	_LamiPress = 5;
	_Scratch = 6;
	_LScratch = 7;
	_Kkopchim = 8;
	_Group = 9;
	_Mura = 10;
	_TacMura = 11;
	_TacMuraB = 12;
	_TacLine = 13;
	_SpotKipo = 14;
	_Line = 15;
	_LineAngle = 16;
	_LineTD = 17;
	_KipoPress = 18;
	_LineKipo = 19;
	_BlackMura = 20;
	_blackGroup = 21;
	_SurfaceBumpy = 22;
}

DefectType::~DefectType()
{

}

void DefectType::Save()
{

}

void DefectType::Load()
{

}

