#include "Param.h"
#include "../stdafx.h"
#include <string>

Param::Param()
{
	ResetAddress();
}

Param::~Param()
{

}

void Param::InitParam()
{
	Common.Init();
	OneUp.Init();
	OneDn.Init();

	HoleSkip.Init();

	memset(_UseLv, 0x00, sizeof(bool) * MAX_LEVEL);
	memset(_Cycle, 0x00, sizeof(bool) * MAX_LEVEL);

	if (_pCheckFishEyeArea != nullptr) *_pCheckFishEyeArea = 0;
	if (_pPinhole != nullptr)	_pPinhole->Init();
	if (_pLvUp != nullptr)	_pLvUp->Init();
	if (_pLvDn != nullptr)	_pLvDn->Init();
	if (_pGroup != nullptr) _pGroup->Init();
	if (_pCunic != nullptr) _pCunic->Init();
	if (_pSC != nullptr) _pSC->Init();
	if (_pLongSC != nullptr) _pLongSC->Init();
	if (_pPress != nullptr) _pPress->Init();
	if (_pBCR != nullptr) _pBCR->Init();
	if (_pKipo != nullptr) _pKipo->Init();
}

void Param::SetParam()
{
	InitParam();

	Common.useRandomCycle =			_Data[20] == 0 ? false : true;
	Common.useAi =					_Data[21] == 0 ? false : true;
	Common.useFlatImage =		(bool)_Data[22];
	HoleSkip.Th =			   (int)_Data[23];
	HoleSkip.Period =				_Data[24];
	HoleSkip.Size =					_Data[25];
	HoleSkip.RemoveTh =				(int)_Data[26];
	HoleSkip.ErrRatio = 25;
	HoleSkip.CompactMin = 0.7f;
	HoleSkip.CompactMax = 0.85f;
	HoleSkip.Maxdiff = 0.4f;	// 50um 기준 2픽셀

	Common.GrabBright =			(int)_Data[29];
	
	Common.TrgGrayDiff =		(int)_Data[30];

	Common.PeriodLevel =		(int)_Data[33];
	if (Common.PeriodLevel > MAX_LEVEL) Common.PeriodLevel = MAX_LEVEL;

	Common.SizeMethod =			(int)_Data[34];
	Common.AttachPxl = 2;

	Common.NoInspection =		(int)_Data[36];
	
	Common.AlgorithmType =		(int)_Data[40];

	Common.NotInspArea =		(int)_Data[42];
	Common.EdgeTh =				(int)_Data[43];
	Common.EdgeOffset =			(int)_Data[44];
	if (Common.EdgeTh > 0)		Common.AutoEdgeFind = 1;
	else						Common.AutoEdgeFind = 0;

	Common.EdgeDir = 0;
	if (Common.NotInspArea > 0)	Common.EdgeDir = 0;			// 왼쪽 못씀
	else if (Common.NotInspArea < 0) Common.EdgeDir = 1;	// 오른쪽 못 씀
	else
	{
		if (Common.EdgeOffset > 0)		Common.EdgeDir = 0;	// 왼쪽 못 씀
		else if (Common.EdgeOffset < 0)	Common.EdgeDir = 1; // 오른쪽 못 씀
	}

	Common.BrightMax =			(int)_Data[49];	// 최대 밝기
	Common.BrightMin =			(int)_Data[50];	// 최소 밝기

	Common.TapeToOpticDist =	(int)_Data[51];			// 광학계에서 테입까지 거리
	

	if (Cam.ScaleX <= 0) Cam.ScaleX = 0.05f;
	if (Cam.ScaleY <= 0) Cam.ScaleY = 0.05f;

	for (int i = 0; i < MAX_LEVEL; i++)
		_UseLv[i] = _Data[60 + i] == 0 ? false : true;
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

void Param::Save(TempParam::PC_INFO* pcInfo, std::wstring path, SystemParam* pSysParam)
{

}

bool Param::Load(TempParam::PC_INFO* pcInfo, std::wstring path, SystemParam *pSysParam)
{
	bool isRes = true;

	int nRet = 0;

	std::wstring str, sParam;
	std::wstring ModelName, strModel;
	std::wstring sParameterPath;
	str.append(pcInfo->Name);
	str.append(L" - Inspect Parameter");

	int i, j, nOptic;
	double dParam[220];
	int nCycle[15];
	double  dSum = 0;

	wchar_t sReturn[20], sName[200];
	

	//SYSTEM DATA------------------------------------------------------------------------------------------------------------
	int shutter;
	double cal_x, cal_y, frame_len, scale_x, scale_y;

	sParameterPath.append(path);
	str.clear();
	str.append(pcInfo->Name);
	str.append(L" - System Parameter");

	::GetPrivateProfileString(str.c_str(), L"shutter", L"3000", sReturn, 10, sParameterPath.c_str()); shutter = (int)_wtof(sReturn);
	::GetPrivateProfileString(str.c_str(), L"cal_x", L"0", sReturn, 10, sParameterPath.c_str()); cal_x = _wtof(sReturn);
	::GetPrivateProfileString(str.c_str(), L"cal_y", L"0", sReturn, 10, sParameterPath.c_str()); cal_y = _wtof(sReturn);
	::GetPrivateProfileString(str.c_str(), L"frame_len", L"100", sReturn, 10, sParameterPath.c_str()); frame_len = _wtof(sReturn);
	::GetPrivateProfileString(str.c_str(), L"scale_x", L"0.05", sReturn, 10, sParameterPath.c_str()); scale_x = _wtof(sReturn);
	::GetPrivateProfileString(str.c_str(), L"scale_y", L"0.05", sReturn, 10, sParameterPath.c_str()); scale_y = _wtof(sReturn);

	Cam.Exposure = shutter;
	Cam.ScaleX = scale_x;
	Cam.ScaleY = scale_y;
	Cam.SttPosX = cal_x;
	Cam.SttPosY = cal_y;
	Cam.FrameLength = frame_len;
	Cam.ImageAngleFactoryY = 0;

	if (shutter < 10 || shutter>100000 || cal_x < -1000 || cal_x>3000 || frame_len < 10 || frame_len>1000 ||
		scale_x < 0.001 || scale_x>1.0 || scale_y < 0.001 || scale_y>1.0) goto done;

	//------------------------------------------------------------------------------------------------------------------------

	//param1~220 까지 읽는다 ------------------------------------------------------------
	str.clear();
	str.append(pcInfo->Name);
	str.append(L" - Inspect Parameter");

	for (i = 0; i < 220; i++)
	{
		sParam.clear();
		sParam.append(L"param");
		sParam.append(std::to_wstring(i + 1));
		::GetPrivateProfileString(str.c_str(), sParam.c_str(), L"0", sReturn, 10, sParameterPath.c_str());

		dParam[i] = _wtof(sReturn);

	//	if (i == 34 || i == 35 || (i >= 37 && i < 40) || (i >= 41 && i < 45) || i == 50) continue; //34~35:물리적마킹거리, 41~44: CamYPos, 미검영역, 자동Edge, TACEdge, 50:최소밝기

		dSum += dParam[i];
	}

	// 데이터 복사
	// 추후 개별 Param class에서 데이터 실제 적용이 일어남.
	memcpy(_Data, dParam, sizeof(double) * 220);


	//SERVER 데이타 읽는다 --------------------------------------------------------------
	str.clear();
	str.append(L"SERVER");

	::GetPrivateProfileString(str.c_str(), L"MachineName", L"", sName, 200, sParameterPath.c_str());
	_MachineName = std::wstring(sName);
	
	::GetPrivateProfileString(str.c_str(), L"strModel", L"", sName, 200, sParameterPath.c_str());
	sParam = std::wstring(sName);
	if (sParam.length() < 1)
	{
		::GetPrivateProfileString(str.c_str(), L"ModelName", L"", sName, 200, sParameterPath.c_str());
		_ModelName = std::wstring(sName);
	}
	else
		_ModelName = std::wstring(sName);


	for (i = 0; i < MAX_CYCLE; i++)
	{
		sParam.clear();
		sParam.append(L"nCycle");
		sParam.append(std::to_wstring(i + 1));
		::GetPrivateProfileString(str.c_str(), sParam.c_str(), L"0", sReturn, 10, sParameterPath.c_str());
		_Cycle[i] = (float)_wtof(sReturn);
		dSum += _Cycle[i];
	}

	if (dSum < 0) dSum *= -1.0;

	nRet = ((int)dSum) % 256;
	if (nRet == 0) nRet = 1;
done:

	SetParam();

	// 시스템 데이터 적용해야하는 데이터 여기서 처리.. -_-; 
	

	return isRes;
}

eParamType::eType Param::GetParamType(std::wstring path, std::wstring section)
{
	wchar_t data[256];
	GetPrivateProfileString(section.c_str(), L"param41", L"1", data, 256, path.c_str());
	return (eParamType::eType) ((int)_wtof(data));
}