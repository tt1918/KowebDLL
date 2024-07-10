#pragma once
#include "../Param/Param.h"
#include "../Param/TempParam.h"
#include "../PreProcess/PreProc.h"

class Inspect
{
private:
	Param*					_pParam;		// 검사 파라미터 (주소만 얻어옴)
	TempParam::TEMP_PARAM*	_pTmpParam;		// 검사 임시 파라미터 (주소만 얻어옴)

	FlatImage*				_pFlatImg;		// 이미지 평활화 클래스
	Profile*				_pProfile;		// 이미지 프로파일 클래스
	FindEdge*				_pEdgeFinder;	// 에지 검출 클래스	

public:
	Inspect();
	virtual ~Inspect();

	void Init();

	void SetParam(Param* pParam) { _pParam = pParam; }
	void SetTempParam(TempParam::TEMP_PARAM* pTmpParam) { _pTmpParam = pTmpParam; }

	virtual void Create();
	virtual void Release();

	virtual void Run();


	void SpotInspect();
};

