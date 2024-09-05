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
	// ����
	SystemParam				*_pSysParam;
	
	// ����
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// ��ó�� 
	FlatImage*				_pFlat;				// �̹��� ��Ȱȭ Ŭ����
	Profile*				_pProfile;			// �̹��� �������� Ŭ����
	PyramidImage*			_pPyramid;			// �Ƕ�̵� ���� ����
	PyramidImage*			_pPyramidSc;		// �Ƕ�̵� ���� ����
	// ��ó�� 
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// �˻�
	IMG_INFO				_ImgInfo;	// �̹����� �������� ������ ������ ���(���� ����. ����)

	Param					*_pParam;
	TempParam::TEMP_PARAM	*_pTempParam;
	Inspect					*_pInspect;
	MARK_OPTIC				*_pMarkParam;
	// �˻�
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


	// ���� Ÿ�Ը� ��������. �߰� ������ �Է��� �˾Ƽ� ó����. 
	void SetParam(eParamType::eType type, InspParam::COMMON_PARAM* pCommon, InspParam::CAM_PARAM* pCamParam,
		InspParam::HOLE_SKIP_PARAM* pHoleSkip, bool* pLevel, float* pCycle, InspParam::PINHOLE_PARAM* pPinhole,
		InspParam::SPOT_PARAM* pLvUp, InspParam::SPOT_PARAM* pLvDn, InspParam::GROUP_PARAM* pGroup, InspParam::CUNIC_PARAM* pCunic,
		InspParam::SC_PARAM* pSC, InspParam::LONG_SC_PARAM* pLongSC, InspParam::PRESS_PARAM* pPress);

	// �˻� �̹����� �־��ش�.
	// �̹��� �����ʹ� ������ ���縸 �Ѵ�. 
	void SetImage(unsigned char* pBuf, int width, int height, int pitch);

	// �Ķ���͸� �Է��Ѵ�. 

	// �˻� ó���Ѵ�. 
	void Run();

	// �ҷ� ���� �����͸� ���� �´�. 
	DEFECTDATA* GetDefectInfo() { return _pInspect->GetDefectData(); }

};

