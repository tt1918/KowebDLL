#pragma once

#include "Inspect\Inspect.h"
#include "Param/CosParam.h"
#include "Param/TempParam.h"
#include "Param/SystemParam.h"
#include "Inspect/InspectCOS.h"
#include "Param/MarkParam.h"

class Inspector
{
public:
	int						_NumOfProc;		// 생성할 Inspect Process 갯수

	/////////////////////////////////////////////////////////////////////
	// 전처리 
	SystemParam				_sysParam;
	// 전처리 
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// 전처리 
	FlatImage*				_pFlat;				// 이미지 평활화 클래스
	Profile*				_pProfile;			// 이미지 프로파일 클래스
	FindEdge*				_pEdgeFinder;		// 에지 검출 클래스	
	PyramidImage*			_pPyramid;			// 피라미드 영상 생성
	PyramidImage*			_pPyramidSc;		// 피라미드 영상 생성
	// 전처리 
	/////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////
	// 검사
	int					_SrcW;		// 원본 영상 W
	int					_SrcH;		// 원본 영상 H
	unsigned char		*_pSrcImg;	// 원본 이미지 영상

	Param**					_pParam;
	TempParam::TEMP_PARAM*	_tempParam;
	Inspect**				_pInspect;
	MARK_OPTIC*				_pMarkParam;
	// 검사
	/////////////////////////////////////////////////////////////////////

public:
	Inspector();
	~Inspector();

private:
	void Release();

public:

	void Initialize(int numOfProc);

	// CosWeb과 다르게 1개의 PC에서 여러개의 프로세스를 생성할 수 있어서 이름은 
	// 입력할 수 있도록 함. 
	void SetParam(int procNum, std::wstring pcName, std::wstring path);

	// 검사 이미지를 넣어준다.
	void SetImage(unsigned char* pBuf, int width, int height, int pitch);

	// 검사 처리한다. 
	void RunWebInspect(int procNum);
};