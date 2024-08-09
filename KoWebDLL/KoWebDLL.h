#pragma once
#include <vector>
#include "PreProcess/FindEdge.h"

#ifndef KOWEBDLL_EXPORTS
#define DLL_EXPORT_API __declspec(dllimport)
#else
#define DLL_EXPORT_API __declspec(dllexport)
#endif

// KoWebDLL handle�� �����Ѵ�.
extern "C" DLL_EXPORT_API void* InitKoWebDll();

// KoWebDLL Handle�� �����Ѵ�.
extern "C" DLL_EXPORT_API void CloseKoWebDll(void* pHandle);

// Dll ���μ��� �ʱ�ȭ �����Ѵ�. ���� ó�� ���μ��� ������ ������
extern "C" DLL_EXPORT_API bool Initialize(void* pHandle, int procCnt);

// �˻� �Ķ���� �ʱ�ȭ�� �����Ѵ�.
extern "C" DLL_EXPORT_API bool SetParam(void* pHandle, int procNum, const wchar_t*pcName, const wchar_t *paramPath);

// �˻��� ������ �Է��Ѵ�.
extern "C" DLL_EXPORT_API int SetImage(void* pHandle, unsigned char* pBuf, int width, int height, int pitch);

// �˻縦 �����Ѵ�.
extern "C" DLL_EXPORT_API int RunWebInspect(void* pHandle, int procNum);

// ���� �˻縦 �����Ѵ�.
// ���� �˻翡�� �˻� ���� Ž���� ���� ��ó�� ������ ���Եȴ�. 
extern "C" DLL_EXPORT_API bool EdgeFind(unsigned char* image, int width, int height, int pitch, int depth, double scaleX, double scaleY,
	int inspMethod, int prodCnt, double prodSize, double prodGap, double refCenterX[],
	int edgeTH, int edgeCnt, FIND_EDGE_RESULT *res);

extern "C" DLL_EXPORT_API bool EdgeFind1(unsigned char* image, int width, int height, int pitch, int depth, double scaleX, double scaleY,
	int inspMethod, int prodCnt, double prodSize, double prodGap, double refCenterX[],
	int edgeTH, int edgeCnt, FIND_EDGE_RESULT * res, int size, int64_t& dataPtr);