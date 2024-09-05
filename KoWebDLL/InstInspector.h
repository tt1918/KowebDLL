#pragma once

#include "Inspect\Inspect.h"
#include "Param/CosParam.h"
#include "Param/TempParam.h"
#include "Param/SystemParam.h"
#include "Inspect/InspectCOS.h"
#include "Param/MarkParam.h"

typedef struct _ImgInfo
{
	unsigned char* pImg;
	int width;
	int height;
	int pitch;

	void Reset()
	{
		pImg = nullptr;
		width = 0;
		height = 0;
		pitch = 0;
	}

}IMG_INFO;

class InstInspector
{
	int _ProcIdx;

	/////////////////////////////////////////////////////////////////////
	// 정보
	SystemParam				*_pSysParam;
	
	// 정보
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// 전처리 
	FlatImage*				_pFlat;				// 이미지 평활화 클래스
	Profile*				_pProfile;			// 이미지 프로파일 클래스
	PyramidImage*			_pPyramid;			// 피라미드 영상 생성
	PyramidImage*			_pPyramidSc;		// 피라미드 영상 생성
	// 전처리 
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// 검사
	IMG_INFO				_ImgInfo;	// 이미지는 상위에서 보내준 포인터 사용(복사 안함. 느림)

	Param					*_pParam;
	TempParam::TEMP_PARAM	*_pTempParam;
	Inspect					*_pInspect;
	MARK_OPTIC				*_pMarkParam;
	// 검사
	/////////////////////////////////////////////////////////////////////

public:
	InstInspector();
	~InstInspector();

private:
	void Release();

public:

	void Initialize(int numOfProc, int width, int height, int pitch);

	void SetSysParam(int width, int height, int pitch);

	void SetEdgeInfo(double X1, double X2, double offsetX1, double offsetX2);

	TempParam::CROP_INFO* GetCropInfo() { return &(_pTempParam->CropInfo); }


	// 생성 타입만 결정해줌. 추가 데이터 입력은 알아서 처리함. 
	void SetParam(eParamType::eType type, InspParam::COMMON_PARAM* pCommon, InspParam::CAM_PARAM* pCamParam,
		InspParam::HOLE_SKIP_PARAM* pHoleSkip, bool* pLevel, float* pCycle, InspParam::PINHOLE_PARAM* pPinhole,
		InspParam::SPOT_PARAM* pLvUp, InspParam::SPOT_PARAM* pLvDn, InspParam::GROUP_PARAM* pGroup, InspParam::CUNIC_PARAM* pCunic,
		InspParam::SC_PARAM* pSC, InspParam::LONG_SC_PARAM* pLongSC, InspParam::PRESS_PARAM* pPress);

	// 검사 이미지를 넣어준다.
	// 이미지 데이터는 포인터 복사만 한다. 
	void SetImage(unsigned char* pBuf, int width, int height, int pitch);

	// 파라미터를 입력한다. 

	// 검사 처리한다. 
	void Run();

	// 불량 정보 데이터를 갖고 온다. 
	DEFECTDATA* GetDefectInfo() { return _pInspect->GetDefectData(); }

};

