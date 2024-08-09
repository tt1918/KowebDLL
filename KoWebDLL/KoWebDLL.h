#pragma once
#include <vector>
#include "PreProcess/FindEdge.h"

#ifndef KOWEBDLL_EXPORTS
#define DLL_EXPORT_API __declspec(dllimport)
#else
#define DLL_EXPORT_API __declspec(dllexport)
#endif

// KoWebDLL handle을 생성한다.
extern "C" DLL_EXPORT_API void* InitKoWebDll();

// KoWebDLL Handle을 제거한다.
extern "C" DLL_EXPORT_API void CloseKoWebDll(void* pHandle);

// Dll 프로세스 초기화 진행한다. 내부 처리 프로세스 갯수를 정의함
extern "C" DLL_EXPORT_API bool Initialize(void* pHandle, int procCnt);

// 검사 파라미터 초기화를 진행한다.
extern "C" DLL_EXPORT_API bool SetParam(void* pHandle, int procNum, const wchar_t*pcName, const wchar_t *paramPath);

// 검사할 영상을 입력한다.
extern "C" DLL_EXPORT_API int SetImage(void* pHandle, unsigned char* pBuf, int width, int height, int pitch);

// 검사를 진행한다.
extern "C" DLL_EXPORT_API int RunWebInspect(void* pHandle, int procNum);

// 에지 검사를 진행한다.
// 실제 검사에서 검사 영역 탐색을 위한 전처리 과정에 포함된다. 
extern "C" DLL_EXPORT_API bool EdgeFind(unsigned char* image, int width, int height, int pitch, int depth, double scaleX, double scaleY,
	int inspMethod, int prodCnt, double prodSize, double prodGap, double refCenterX[],
	int edgeTH, int edgeCnt, FIND_EDGE_RESULT *res);

extern "C" DLL_EXPORT_API bool EdgeFind1(unsigned char* image, int width, int height, int pitch, int depth, double scaleX, double scaleY,
	int inspMethod, int prodCnt, double prodSize, double prodGap, double refCenterX[],
	int edgeTH, int edgeCnt, FIND_EDGE_RESULT * res, int size, int64_t& dataPtr);