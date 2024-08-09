// KoWebDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KoWebDLL.h"
#include "Inspector.h"
#include "PreProcess/FindEdge.h"

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

extern "C" DLL_EXPORT_API bool EdgeFind1(unsigned char* image, int width, int height, int pitch, int depth, double scaleX, double scaleY,
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
