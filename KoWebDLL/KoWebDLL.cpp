// KoWebDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KoWebDLL.h"
#include "Inspector.h"

extern "C" DLL_EXPORT_API void* InitKoWebDll()
{
	void *pHandle = nullptr;
	Inspector* inspector = new Inspector();
	pHandle = (void*)inspector;
	
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