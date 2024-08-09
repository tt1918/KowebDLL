// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//#define USE_OPENCV					// 연산에 OPEN_CV 사용

#ifdef USE_OPENCV
#ifdef _DEBUG
#pragma comment(lib, "OpenCV\\lib\\opencv_world470d.lib")
#pragma comment(lib, "OpenCV\\lib\\opencv_ts470d.lib")
#pragma comment(lib, "OpenCV\\lib\\opencv_img_hash470d.lib")
#else
#pragma comment(lib, "OpenCV\\lib\\opencv_world470.lib")
#pragma comment(lib, "OpenCV\\lib\\opencv_ts470.lib")
#pragma comment(lib, "OpenCV\\lib\\opencv_img_hash470.lib")
#endif
#endif


// TODO: reference additional headers your program requires here
