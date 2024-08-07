#pragma once

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