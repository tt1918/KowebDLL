#pragma once

#ifndef KOWEBDLL_EXPORTS
#define DLL_EXPORT_API __declspec(dllimport)
#else
#define DLL_EXPORT_API __declspec(dllexport)
#endif

// KoWebDLL handle을 생성한다.
extern "C" DLL_EXPORT_API void* InitKoWebDll();

// KoWebDLL Handle을 제거한다.
extern "C" DLL_EXPORT_API void CloseKoWebDll(void* pHandle);