#pragma once
#include <vector>
#include "PreProcess/FindEdge.h"
#include "Defect/DefectData.h"

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
	int scVal1, int ScVal2, int ScVal3, int ScVal4, int ScVal5, int ScVal6, int ScVal7, int ScVal8, int ScVal9, int ScVal10);

extern "C" DLL_EXPORT_API int RunBasicWebInsp(int procNum, unsigned char* image, int width, int height, int pitch, double scaleX, double scaleY, double edgeX1, double edgeX2, double edgeOffset,
	int brightMin, int brightMax, int sizeMethod, int noInsp,
	bool useLv[], float cycVal[],
	int upAreaTh, int lvUpTh[], double upSize[],
	int dnAreaTh, int lvDnTh[], double dnSize[],
	int cnInspArea, double cnCandiVal, double cnStd[],
	int psUpTh, int psDnTh, int psUpAreaTh, int psDnAreaTh, int psVal[], double psSize[], int scVal[],
	DEFECTDATA * pDefect);