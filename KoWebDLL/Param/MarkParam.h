#pragma once
#include "../Defines.h"
#include <string>

typedef struct _stMarkOptic
{
	int Data[MAX_OPTIC][MAX_LEVEL];		// 마킹데이타 (1 마킹함, 0 마킹안함)   [10:Defect종류][10:Level]
	int LvMCnt[MAX_OPTIC][MAX_LEVEL];		// Defect종류, 10 Level의  M당 불량수
	int DefectMCnt[MAX_OPTIC];		// Defect종류별 같은 X에서의 불량수
	float BaseLength_mm;	// M 당 불량수 구하는 기준 길이
	float BaseLength_M;		// 큰 영역 불량수 구하는 기준 길이
	float BaseX_mm;			// 큰 영역 같은 불량 간주 +/- 폭
	int	USE[2];				// 마킹기 사용 여부


	int CheckLvMCnt;		// 가변마킹 1단계(군집성) 사용하면 1
	int CheckDefectMCnt;	// 가변마킹 2단계(주기성) 사용하면 1
	int Marking[MAX_OPTIC][MAX_LEVEL];	// 최종 마킹 (1 마킹함, 0 마킹안함)    [10:Defect종류][10:Level]

	bool Load(TempParam::PC_INFO* pcInfo, std::wstring path)
	{
		bool isRes = true;

		int nRet = 0;

		std::wstring str, sParam;
		std::wstring ModelName, strModel;
		std::wstring sParameterPath;

		int i, j, nOptic;
		int nTmp, nCnt;
		wchar_t sReturn[20];

		CheckLvMCnt = 0;
		CheckDefectMCnt = 0;

		nCnt = 0;
		nOptic = pcInfo->Optic;
		for (i = nOptic * MAX_LEVEL; i < nOptic * MAX_LEVEL + MAX_LEVEL; i++)
		{
			for (j = 0; j < MAX_LEVEL; j++)
			{
				sParam.clear();
				sParam.append(L"nData[");
				sParam.append(std::to_wstring(i));
				sParam.append(L"][");
				sParam.append(std::to_wstring(j));
				sParam.append(L"]");

				::GetPrivateProfileString(L"MARKING", sParam.c_str(), L"0", sReturn, 10, sParameterPath.c_str());
				Data[nCnt][j] = _wtoi(sReturn);
			}
			nCnt++;
		}

		nCnt = 0;
		for (i = nOptic * MAX_LEVEL; i < nOptic * MAX_LEVEL + MAX_LEVEL; i++)
		{
			for (j = 0; j < MAX_LEVEL; j++)
			{
				sParam.clear();
				sParam.append(L"nLevelMCount[");
				sParam.append(std::to_wstring(i));
				sParam.append(L"][");
				sParam.append(std::to_wstring(j));
				sParam.append(L"]");

				::GetPrivateProfileString(L"MARKING", sParam.c_str(), L"0", sReturn, 10, sParameterPath.c_str());
				LvMCnt[nCnt][j] = _wtoi(sReturn);

				if (LvMCnt[nCnt][j] > 0 && LvMCnt[nCnt][j] < 9999) CheckLvMCnt = 1;
			}
			nCnt++;
		}

		//불량종류별 최대 데이타 수------------------------------------------------------------
		nCnt = 0;
		for (i = 0; i < MAX_OPTIC; i++) //불량종류
		{
			sParam.clear();
			sParam.append(L"nLevelMCount[");
			sParam.append(std::to_wstring(nOptic));
			sParam.append(L"][");
			sParam.append(std::to_wstring(i));
			sParam.append(L"]");

			::GetPrivateProfileString(L"MARKING", sParam.c_str(), L"0", sReturn, 10, sParameterPath.c_str());
			DefectMCnt[nCnt++] = _wtoi(sReturn);

			if (DefectMCnt[nCnt - 1]) CheckDefectMCnt = 1;
		}
		//-------------------------------------------------------------------------------------

		::GetPrivateProfileString(L"MARKING", L"dBaseLength_mm", L"1000", sReturn, 10, sParameterPath.c_str());
		BaseLength_mm = (float)(_wtof(sReturn));
		if (BaseLength_mm < 100)  BaseLength_mm = 100;
		if (BaseLength_mm > 5000) BaseLength_mm = 5000;

		::GetPrivateProfileString(L"MARKING", L"dBaseLength_M", L"30", sReturn, 10, sParameterPath.c_str());
		BaseLength_M = (float)(_wtof(sReturn));
		if (BaseLength_M < 1)   BaseLength_M = 1;
		if (BaseLength_M > 300) BaseLength_M = 300;

		::GetPrivateProfileString(L"MARKING", L"dBaseX_mm", L"10", sReturn, 10, sParameterPath.c_str());
		BaseX_mm = (float)(_wtoi(sReturn));
		if (BaseX_mm < 2) BaseX_mm = 2;

		::GetPrivateProfileString(L"MARKING", L"nUSE1", L"0", sReturn, 10, sParameterPath.c_str());
		nTmp = _wtoi(sReturn);
		if (nTmp & (0x01 << nOptic))	USE[0] = 1;
		else			 				USE[0] = 0;

		::GetPrivateProfileString(L"MARKING", L"nUSE2", L"0", sReturn, 10, sParameterPath.c_str());
		nTmp = _wtoi(sReturn);
		if (nTmp & (0x01 << nOptic))	USE[1] = 1;
		else			 				USE[1] = 0;

		//최종 g_Mark.nLevelMCount값 한개만 이용하면 된다.
		for (i = 0; i < MAX_OPTIC; i++)
			memcpy(Marking[i], Data[i], sizeof(int) * MAX_LEVEL);

		return true;
	}
}MARK_OPTIC;

