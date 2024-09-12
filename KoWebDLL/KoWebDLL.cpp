// KoWebDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KoWebDLL.h"
#include "Inspector.h"
#include "PreProcess/FindEdge.h"
#include "InstInspector.h"
#include "atlimage.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class 구현용 - S
extern "C" DLL_EXPORT_API void* InitKoWebDll()
{
	void *pHandle = nullptr;
	Inspector* pInsp = new Inspector();
	pHandle = (void*)pInsp;
	
	return pHandle;
}

extern "C" DLL_EXPORT_API void CloseKoWebDll(void* pHandle)
{
	if (pHandle != nullptr)
	{
		delete (Inspector *) pHandle;
		pHandle = nullptr;
	}
}

extern "C" DLL_EXPORT_API bool Initialize(void* pHandle, int procCnt)
{
	if (pHandle == nullptr)
		return false;

	Inspector* pInsp = (Inspector*)pHandle;
	pInsp->Initialize(procCnt);

	return true;
}


extern "C" DLL_EXPORT_API bool SetParam(void* pHandle, int procNum, const wchar_t *pcName, const wchar_t* paramPath)
{
	if (pHandle == nullptr)
		return false;

	Inspector* pInsp = (Inspector*)pHandle;
	
	std::wstring pcNameA, paramPathA;
	pcNameA = std::wstring(pcName);
	paramPathA = std::wstring(paramPath);

	pInsp->SetParam(procNum, pcNameA, paramPathA);

	return true;
}

extern "C" DLL_EXPORT_API int SetImage(void* pHandle, unsigned char* pBuf, int width, int height, int pitch)
{
	if (pHandle == nullptr)
		return -1;
	
	Inspector* pInsp = (Inspector*)pHandle;

	int srcW = pInsp->_SrcW;
	int srcH = pInsp->_SrcH;

	if (srcW != width) return -2;
	if (srcH != height) return -3;

	// 영상 입력 처리
	pInsp->SetImage(pBuf, width, height, pitch);
	
	return 0;
}

extern "C" DLL_EXPORT_API int RunWebInspect(void* pHandle, int procNum)
{
	if (pHandle == nullptr)
		return -1;

	Inspector* pInsp = (Inspector*)pHandle;

	pInsp->RunWebInspect(procNum);

	return 0;
}
// Class 구현용 - E
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 함수 방식 처리 - S

// 검사 기능 테스트용 함수
extern "C" DLL_EXPORT_API int RunBasicWebInsp_T(int procNum, unsigned char* image, int width, int height, int pitch, double scaleX, double scaleY, double edgeX1, double edgeX2, double edgeOffset,
	int brightMin, int brightMax, int sizeMethod, int noInsp,
	bool useLv1, bool useLv2, bool useLv3, bool useLv4, bool useLv5, bool useLv6, bool useLv7, bool useLv8, bool useLv9, bool useLv10,
	float cycVal1, float cycVal2, float cycVal3, float cycVal4, float cycVal5, float cycVal6, float cycVal7, float cycVal8, float cycVal9, float cycVal10,
	int upAreaTh, int lvUpTh1, int lvUpTh2, int lvUpTh3, int lvUpTh4, int lvUpTh5, int lvUpTh6, int lvUpTh7, int lvUpTh8, int lvUpTh9, int lvUpTh10,
	double upSize1, double upSize2, double upSize3, double upSize4, double upSize5, double upSize6, double upSize7, double upSize8, double upSize9, double upSize10,
	int dnAreaTh, int lvDnTh1, int lvDnTh2, int lvDnTh3, int lvDnTh4, int lvDnTh5, int lvDnTh6, int lvDnTh7, int lvDnTh8, int lvDnTh9, int lvDnTh10,
	double dnSize1, double dnSize2, double dnSize3, double dnSize4, double dnSize5, double dnSize6, double dnSize7, double dnSize8, double dnSize9, double dnSize10,
	int cnInspArea, double cnCandiVal, double cnStd_1, double cnStd_2, double cnStd_3, double cnStd_4, double cnStd_5, double cnStd_6, double cnStd_7, double cnStd_8, double cnStd_9, double cnStd_10,
	int psUpTh, int psDnTh, int psUpAreaTh, int psDnAreaTh, int psVal1, int psVal2, int psVal3, int psVal4, int psVal5, int psVal6, int psVal7, int psVal8, int psVal9, int psVal10,
	double psSize1, double psSize2, double psSize3, double psSize4, double psSize5, double psSize6, double psSize7, double psSize8, double psSize9, double psSize10, 
	int scVal1, int ScVal2, int ScVal3, int ScVal4, int ScVal5, int ScVal6, int ScVal7, int ScVal8, int ScVal9, int ScVal10)
{
	InstInspector Insp;



	unsigned char* pCropImg = nullptr;
	TempParam::CROP_INFO* pCropInfo;	// 주소만 얻어옴.
	//////////////////////////////////////////////////////////////////////
	// 설정 셋팅 - S
	Insp.SetEdgeInfo(edgeX1, edgeX2, edgeOffset, edgeOffset);

	pCropInfo = Insp.GetCropInfo();

	int cropWidth = pCropInfo->width;
	int stX = pCropInfo->StX;
	pCropImg = new unsigned char[(cropWidth * height)];
	for (int i = 0; i < height; i++)
		memcpy(pCropImg + (i * cropWidth), image + (i * width) + stX, sizeof(unsigned char) * cropWidth);

	Insp.Initialize(procNum, cropWidth, height, cropWidth);
	Insp.SetImage(pCropImg, cropWidth, height, cropWidth);

	// 이미지를 다이렉트로 처리할지 Crop처리할지.. 
	InspParam::COMMON_PARAM commonParam;
	InspParam::CAM_PARAM camParam;
	InspParam::HOLE_SKIP_PARAM holeSkipParam;
	bool useLv[10];
	float cycleVal[10];
	InspParam::PINHOLE_PARAM pinholeParam;
	InspParam::GROUP_PARAM groupParam;
	InspParam::SPOT_PARAM upParam;
	InspParam::SPOT_PARAM dnParam;
	InspParam::CUNIC_PARAM cunicParam;
	InspParam::SC_PARAM scParam;
	InspParam::LONG_SC_PARAM longScParam;
	InspParam::PRESS_PARAM pressParam;
	
	commonParam.Init();
	commonParam.AlgorithmType = eParamType::eCOS2;
	commonParam.BrightMin = brightMin;
	commonParam.BrightMax = brightMax;
	commonParam.SizeMethod = sizeMethod;
	commonParam.NoInspection = noInsp;

	camParam.Init();
	camParam.ScaleX = scaleX;
	camParam.ScaleY = scaleY;
	camParam.FrameLength = scaleY * height;

	holeSkipParam.Init();

	useLv[0] = useLv1;
	useLv[1] = useLv2;
	useLv[2] = useLv3;
	useLv[3] = useLv4;
	useLv[4] = useLv5;
	useLv[5] = useLv6;
	useLv[6] = useLv7;
	useLv[7] = useLv8;
	useLv[8] = useLv9;
	useLv[9] = useLv10;

	cycleVal[0] = cycVal1;
	cycleVal[1] = cycVal2;
	cycleVal[2] = cycVal3;
	cycleVal[3] = cycVal4;
	cycleVal[4] = cycVal5;
	cycleVal[5] = cycVal6;
	cycleVal[6] = cycVal7;
	cycleVal[7] = cycVal8;
	cycleVal[8] = cycVal9;
	cycleVal[9] = cycVal10;

	pinholeParam.Init();
	groupParam.Init();

	upParam.Init();
	upParam.AreaTh = upAreaTh;
	upParam.SetLvTH(lvUpTh1, lvUpTh2, lvUpTh3, lvUpTh4, lvUpTh5, lvUpTh6, lvUpTh7, lvUpTh8, lvUpTh9, lvUpTh10);
	upParam.SetLvSize(upSize1, upSize2, upSize3, upSize4, upSize5, upSize6, upSize7, upSize8, upSize9, upSize10);

	dnParam.Init();
	dnParam.AreaTh = dnAreaTh;
	dnParam.SetLvTH(lvDnTh1, lvDnTh2, lvDnTh3, lvDnTh4, lvDnTh5, lvDnTh6, lvDnTh7, lvDnTh8, lvDnTh9, lvDnTh10);
	dnParam.SetLvSize(dnSize1, dnSize2, dnSize3, dnSize4, dnSize5, dnSize6, dnSize7, dnSize8, dnSize9, dnSize10);

	cunicParam.Init();
	cunicParam.SetInspArea(cnInspArea, cnCandiVal);
	cunicParam.SetStd(cnStd_1, cnStd_2, cnStd_3, cnStd_4, cnStd_5, cnStd_6, cnStd_7, cnStd_8, cnStd_9, cnStd_10);

	pressParam.Init();
	pressParam.SetBaseParam(psUpTh, psDnTh, psDnAreaTh, psDnAreaTh);
	pressParam.SetValue(psVal1, psVal2, psVal3, psVal4, psVal5, psVal6, psVal7, psVal8, psVal9, psVal10);
	pressParam.SetSize(psSize1, psSize2, psSize3, psSize4, psSize5, psSize6, psSize7, psSize8, psSize9, psSize10);

	scParam.Init();
	scParam.SetValue(scVal1, ScVal2, ScVal3, ScVal4, ScVal5, ScVal6, ScVal7, ScVal8, ScVal9, ScVal10);

 	Insp.SetParam(eParamType::eCOS2, &commonParam, &camParam, &holeSkipParam, useLv, cycleVal, &pinholeParam, &upParam,
		&dnParam, &groupParam, &cunicParam, &scParam, &longScParam, &pressParam);
	// 설정 셋팅 - E
	//////////////////////////////////////////////////////////////////////

	Insp.Run();

	CString strTemp;
	DEFECTDATA* pSrcDefect = Insp.GetDefectInfo();
	
	strTemp.Format(_T("Defect Count = %d\n"), pSrcDefect->Count);

	OutputDebugString(strTemp);

	for (int i = 0; i < pSrcDefect->Count; i++)
	{
		strTemp.Format(_T("PosX = %0.3f, posY = %0.3f\n"), pSrcDefect->pInfo[i].x_pos, pSrcDefect->pInfo[i].y_pos);
		OutputDebugString(strTemp);
	}

	if (pCropImg != nullptr)
		delete[] pCropImg;

	return 0;
}

// CPP 사용 검사 함수
// 기본 검사는 COS-2 기준으로 검사 진행함.
extern "C" DLL_EXPORT_API int RunBasicWebInsp(int procNum, unsigned char* image, int width, int height, int pitch, double scaleX, double scaleY, double edgeX1, double edgeX2, double edgeOffset, 
							int brightMin, int brightMax, int sizeMethod, int noInsp,
							bool useLv[], float cycVal[],
							int upAreaTh, int lvUpTh[], double upSize[],
							int dnAreaTh, int lvDnTh[], double dnSize[],
							int cnInspArea, double cnCandiVal, double cnStd[],
							int psUpTh, int psDnTh, int psUpAreaTh, int psDnAreaTh, int psVal[],double psSize[], int scVal[],
							DEFECTDATA* pDefect)
{
	InstInspector Insp;

	

	unsigned char* pCropImg = nullptr;
	TempParam::CROP_INFO* pCropInfo;	// 주소만 얻어옴.
	//////////////////////////////////////////////////////////////////////
	// 설정 셋팅 - S
	Insp.SetEdgeInfo(edgeX1, edgeX2, edgeOffset, edgeOffset);

	pCropInfo = Insp.GetCropInfo();

	// 검사 영역 만큼만 이미지 Crop하여 처리함.
	int cropWidth = pCropInfo->width;
	int stX = pCropInfo->StX;
	pCropImg = new unsigned char[(cropWidth * height)];
	for (int i = 0; i < height; i++)
		memcpy(pCropImg + (i * cropWidth), image + (i * width) + stX, sizeof(unsigned char) * cropWidth);

	Insp.Initialize(procNum, cropWidth, height, cropWidth);
	Insp.SetImage(pCropImg, cropWidth, height, cropWidth);
	
	// 이미지를 다이렉트로 처리할지 Crop처리할지.. 
	InspParam::COMMON_PARAM commonParam;
	InspParam::CAM_PARAM camParam;
	InspParam::HOLE_SKIP_PARAM holeSkipParam;
	
	InspParam::PINHOLE_PARAM pinholeParam;
	InspParam::GROUP_PARAM groupParam;
	InspParam::SPOT_PARAM upParam;
	InspParam::SPOT_PARAM dnParam;
	InspParam::CUNIC_PARAM cunicParam;
	InspParam::SC_PARAM scParam;
	InspParam::LONG_SC_PARAM longScParam;
	InspParam::PRESS_PARAM pressParam;

	commonParam.Init();
	commonParam.AlgorithmType = eParamType::eCOS2;
	commonParam.BrightMin = brightMin;
	commonParam.BrightMax = brightMax;
	commonParam.SizeMethod = sizeMethod;
	commonParam.NoInspection = noInsp;

	camParam.Init();
	camParam.ScaleX = scaleX;
	camParam.ScaleY = scaleY;
	camParam.FrameLength = scaleY * height;

	holeSkipParam.Init();
	pinholeParam.Init();
	groupParam.Init();

	upParam.Init();
	upParam.SetData(upAreaTh, lvUpTh, upSize);

	dnParam.Init();
	dnParam.SetData(dnAreaTh, lvDnTh, dnSize);

	cunicParam.Init();
	cunicParam.SetData(cnInspArea, cnCandiVal, cnStd); 

	pressParam.Init();
	pressParam.SetData(psUpTh, psDnTh, psDnAreaTh, psDnAreaTh, psVal, psSize);

	scParam.Init();
	scParam.SetValue(scVal);

	Insp.SetParam(eParamType::eCOS2, &commonParam, &camParam, &holeSkipParam, useLv, cycVal, &pinholeParam, &upParam,
		&dnParam, &groupParam, &cunicParam, &scParam, &longScParam, &pressParam);
	// 설정 셋팅 - E
	//////////////////////////////////////////////////////////////////////

	Insp.Run();
	DEFECTDATA* pSrcDefect = Insp.GetDefectInfo();

	pDefect->FrameNum = 0;
	pDefect->Count = pSrcDefect->Count;
	memcpy(pDefect->pArea, pSrcDefect->pArea, sizeof(RECT) * pSrcDefect->Count);
	memcpy(pDefect->pInfo, pSrcDefect->pInfo, sizeof(DEFECT) * pSrcDefect->Count);

	for (int i = 0; i < pSrcDefect->Count; i++)
		memcpy(pDefect->pImage[i], pSrcDefect->pImage[i], sizeof(unsigned char) * BAD_IMG_WIDTH * BAD_IMG_HEIGHT);

	if (pCropImg != nullptr)
		delete[] pCropImg;

	return 0;
}

// CPP용 에지 검사 함수
extern "C" DLL_EXPORT_API bool EdgeFind(unsigned char* image, int width, int height, int pitch, int depth, double scaleX, double scaleY,
										int inspMethod, int prodCnt, double prodSize, double prodGap, double refCenterX[],
										int edgeTH, int edgeCnt, FIND_EDGE_RESULT *res)
{
	FindEdge findEdge;
	findEdge.Init(width, height);

	std::vector<FIND_EDGE_RESULT> inspResult;

	bool bRes = findEdge.InspectEdge(image, width, height, pitch, depth, scaleX, scaleY, prodCnt, inspMethod, edgeTH, edgeCnt, prodSize, prodGap, refCenterX, &inspResult);

	for (int i = 0; i < inspResult.size(); i++)
		res[i] = inspResult[i];

	// 데이터 정리
	findEdge.Destroy();
	return bRes;
}


// C_Sharp용 에지 검사 함수 
extern "C" DLL_EXPORT_API bool EdgeFind_Sharp(unsigned char* image, int width, int height, int pitch, int depth, double scaleX, double scaleY,
	int inspMethod, int prodCnt, double prodSize, double prodGap, double refCenterX[],
	int edgeTH, int edgeCnt, FIND_EDGE_RESULT * res, int size, int64_t& dataPtr)
{
	dataPtr = (int64_t)res;
	FindEdge findEdge;
	findEdge.Init(width, height);

	std::vector<FIND_EDGE_RESULT> inspResult;

	int size1 = sizeof(FIND_EDGE_RESULT);

	bool bRes = findEdge.InspectEdge(image, width, height, pitch, depth, scaleX, scaleY, prodCnt, inspMethod, edgeTH, edgeCnt, prodSize, prodGap, refCenterX, &inspResult);

	for (int i = 0; i < inspResult.size(); i++)
		res[i] = inspResult[i];

	// 데이터 정리
	findEdge.Destroy();

	return bRes;
}
// 함수 방식 처리 - E
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
