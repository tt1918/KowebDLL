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