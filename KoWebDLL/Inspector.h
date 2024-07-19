#pragma once

#include "Inspect\Inspect.h"
#include "Param/CosParam.h"
#include "Param/TempParam.h"
#include "Param/SystemParam.h"
#include "Inspect/InspectCOS.h"
class Inspector
{
public:
	/////////////////////////////////////////////////////////////////////
	// 전처리 
	SystemParam				_sysParam;
	Param					*_pParam;
	TempParam::TEMP_PARAM	_tempParam;
	// 전처리 
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// 전처리 
	FlatImage* _pFlat;				// 이미지 평활화 클래스
	Profile* _pProfile;				// 이미지 프로파일 클래스
	FindEdge* _pEdgeFinder;			// 에지 검출 클래스	
	PyramidImage* _pPyramid;		// 피라미드 영상 생성
	PyramidImage* _pPyramidSc;		// 피라미드 영상 생성
	// 전처리 
	/////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////
	// 검사
	int					_SrcW;		// 원본 영상 W
	int					_SrcH;		// 원본 영상 H
	unsigned char		*_pSrcImg;	// 원본 이미지 영상
	Inspect				*_pInspect;
	// 검사
	/////////////////////////////////////////////////////////////////////

public:
	Inspector();
	~Inspector();

	void Initialize();

	void Initialize(std::string path);

	void Release();

	void SetImage(unsigned char* pBuf, int width, int pitch, int height);

	void RunWebInspect();

};