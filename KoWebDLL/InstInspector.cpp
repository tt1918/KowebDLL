#include "InstInspector.h"

#include "atlimage.h"

InstInspector::InstInspector()
{
	_pSysParam = new SystemParam();
	_pParam = nullptr;
	_pInspect = nullptr;

	_ImgInfo.Reset();


	/////////////////////////////////////////////
	// 전처리 클래스
	_pFlat = nullptr;
	_pProfile = nullptr;
	_pPyramid = nullptr;
	_pPyramidSc = nullptr;
	// 전처리 클래스
	/////////////////////////////////////////////

	_pTempParam = new TempParam::TEMP_PARAM();

	_pMarkParam = new MARK_OPTIC();
}

InstInspector::~InstInspector()
{
	Release();
}

void InstInspector::Initialize(int numOfProc, int width, int height, int pitch)
{
	_ProcIdx = numOfProc;

	//Release();

	_pTempParam->PcInfo.FirstNo = _ProcIdx+1;

	SetSysParam(width, height, pitch);
}

void InstInspector::SetSysParam(int width, int height, int pitch)
{
	_pSysParam->SetSystemParamType1(width, height, pitch);
}

void InstInspector::SetEdgeInfo(double X1, double X2, double offsetX1, double offsetX2)
{
	// 에지 설정 후 검사 이미지 크롭 사이즈 설정해야 함. 
	_pTempParam->CropInfo.Set(X1, X2, offsetX1, offsetX2);

	// Crop 기준으로 이미지 정보를 넣어준다. 
	_pTempParam->InspArea.EdgeX1 = 0;
	_pTempParam->InspArea.EdgeX2 = _pTempParam->CropInfo.width;
	_pTempParam->InspArea.Type = 0;
	_pTempParam->InspArea.X1 = _pTempParam->CropInfo.GetCropX1(); // -> 방향으로 + 됨.
	_pTempParam->InspArea.X2 = _pTempParam->CropInfo.GetCropX2(); // <- 방향으로 + 됨. 

}

void InstInspector::SetParam(eParamType::eType type, 
	InspParam::COMMON_PARAM *pCommon, 
	InspParam::CAM_PARAM *pCamParam, 
	InspParam::HOLE_SKIP_PARAM *pHoleSkip, 
	bool *pLevel, 
	float *pCycle, 
	InspParam::PINHOLE_PARAM* pPinhole, 
	InspParam::SPOT_PARAM* pLvUp, 
	InspParam::SPOT_PARAM* pLvDn, 
	InspParam::GROUP_PARAM* pGroup, 
	InspParam::CUNIC_PARAM* pCunic, 
	InspParam::SC_PARAM* pSC, 
	InspParam::LONG_SC_PARAM* pLongSC, 
	InspParam::PRESS_PARAM* pPress)
{
	TempParam::TEMP_PARAM* pTmpParam = _pTempParam;
	
	if (_pParam != nullptr)
	{
		delete _pParam;
		_pParam = nullptr;
	}

	switch (type)
	{
	case eParamType::eCOS1:
		_pParam = new Cos1Param();
		_pInspect = new InspectCOS1();
		break;
	case eParamType::eCOS2:
		_pParam = new Cos2Param();
		((Cos2Param*)_pParam)->SetInstParam(pCommon, pCamParam, pHoleSkip, pLevel, pCycle, pLvUp, pLvDn, pPress, pCunic, pPinhole, pSC);
		_pInspect = new InspectCOS2();
		break;
	case eParamType::eCOS3:
		_pParam = new Cos3Param();
		_pInspect = new InspectCOS3();
		break;
	case eParamType::eCOS4:
		_pParam = new Cos4Param();
		_pInspect = new InspectCOS4();
		break;
	case eParamType::eCOS5:
		_pParam = new Cos5Param();
		_pInspect = new InspectCOS5();
		break;
	case eParamType::eCOS6:
		_pParam = new Cos6Param();
		_pInspect = new InspectCOS6();
		break;
	case eParamType::eCOS7:
		_pParam = new Cos7Param();
		_pInspect = new InspectCOS7();
		break;
	case eParamType::eCOS8:
		_pParam = new Cos8Param();
		_pInspect = new InspectCOS8();
		break;
	case eParamType::eCOS9:
		_pParam = new Cos9Param();
		_pInspect = new InspectCOS9();
		break;
	case eParamType::eCOSA:
		_pParam = new CosAParam();
		_pInspect = new InspectCOSA();
		break;
	case eParamType::eCOSB:
		_pParam = new CosBParam();
		_pInspect = new InspectCOSB();
		break;
	case eParamType::eCOSC:
		_pParam = new CosCParam();
		_pInspect = new InspectCOSC();
		break;
	case eParamType::eCOSD:
		_pParam = new CosDParam();
		_pInspect = new InspectCOSD();
		break;
	case eParamType::eCOSE:
		_pParam = new CosEParam();
		_pInspect = new InspectCOSE();
		break;
	case eParamType::eCBCR:
		_pParam = new CBCRParam();
		_pInspect = new InspectCOSBCR();
		break;
	}

	Inspect* pIncp = _pInspect;
	Param* pParam = _pParam;
	
	pIncp->Create(_pSysParam->ImageW, _pSysParam->ImageH);

	/////////////////////////////////////////////////////////////////////////////////
	// 평활화 이미지 클래스	 - S
	_pFlat = new FlatImage();
	_pFlat->Init(_pSysParam->ImageW, _pSysParam->ImageH);
	// 평활화 이미지 클래스	 - E
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////
	// 프로파일 생성 클래스	 - S
	_pProfile = new Profile();
	// 프로파일 생성 클래스	 - E
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////
	// 피라미드 이미지 클래스	- S
	// Spot 검사용 
	_pPyramid = new PyramidImage();
	_pPyramid->SetImageInfo(_pSysParam->ImageW, _pSysParam->ImageH);

	// 스크래치 검사용 이미지
	_pPyramidSc = new PyramidImage(2);	// 2배 축소한 이미지만 필요함. 
	_pPyramidSc->SetImageInfo(_pSysParam->ImageW, _pSysParam->ImageH);
	// 피라미드 이미지 클래스	- E
	/////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// 각 전처리 클래스 및 데이터 클래스를 검사 클래스로 연결	- S
	pIncp->SetSrcImage(_ImgInfo.pImg);
	pIncp->SetFlatCls(_pFlat);
	pIncp->SetProfileCls(_pProfile);
	pIncp->SetPyramidCls(_pPyramid, 0);

	pIncp->SetSysParam(_pSysParam);
	pIncp->SetTempParam(pTmpParam);
	pIncp->SetParam(_pParam);
	pIncp->SetMarkParam(_pMarkParam);

	if (pParam->_pSC != nullptr)
	{
		pIncp->SetPyramidCls(_pPyramidSc, 1);
		pIncp->SetScInspBuf(_ImgInfo.width, _ImgInfo.height);
	}
	// 각 전처리 클래스 및 데이터 클래스를 검사 클래스로 연결	- E
	//////////////////////////////////////////////////////////////////////////////////
	
}

void InstInspector::Release()
{
	if (_pFlat != nullptr)
	{
		delete _pFlat;
		_pFlat = nullptr;
	}

	if (_pProfile != nullptr)
	{
		delete _pProfile;
		_pProfile = nullptr;
	}

	if (_pPyramid != nullptr)
	{
		delete _pPyramid;
		_pPyramid = nullptr;
	}

	if (_pPyramidSc != nullptr)
	{
		delete _pPyramidSc;
		_pPyramidSc = nullptr;
	}

	if (_pParam != nullptr)
	{
		delete _pParam;
		_pParam = nullptr;
	}

	if (_pInspect != nullptr)
	{
		delete _pInspect;
		_pInspect = nullptr;
	}

	if (_pSysParam != nullptr)
	{
		delete _pSysParam;
		_pSysParam = nullptr;
	}

	if (_pTempParam != nullptr)
	{
		delete _pTempParam;
		_pTempParam = nullptr;
	}

	if (_pMarkParam != nullptr)
	{
		delete _pMarkParam;
		_pMarkParam = nullptr;
	}
}

// 전체 이미지를 설정할 때. 
void InstInspector::SetImage(unsigned char* pBuf, int width, int height, int pitch)
{
	_ImgInfo.pImg = pBuf;
	_ImgInfo.width = width;
	_ImgInfo.height = height;
	_ImgInfo.pitch = pitch;
}

void InstInspector::Run()
{
	unsigned char* pSrc;

	Param* pParam = _pParam;
	CString strTemp;

	clock_t start = clock();

	_pPyramid->Reset();

	/*int start = GetTickCount();*/

	clock_t time1 = clock();
	_pProfile->MakeProfileData(_ImgInfo.pImg, _ImgInfo.width, _ImgInfo.height, _ImgInfo.pitch);
	clock_t time2 = clock();
	strTemp.Format(L"Elapsed Time 1 : %0.3f", double(time2 - time1) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	clock_t time3 = clock();
	strTemp.Format(L"Elapsed Time 2 : %0.3f", double(time3 - time2) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	
	int nAvgBright = _pProfile->CalcAvgBright(_pTempParam->InspArea.X1, _pTempParam->InspArea.X2, _pSysParam->FlatBright);

	clock_t time4 = clock();
	strTemp.Format(L"Elapsed Time 3 : %0.3f", double(time4 - time3) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	if (pParam->Common.useFlatImage)
	{
		_pFlat->MakeFlatLineScan(_ImgInfo.pImg, _pProfile->GetProfile(), _pSysParam->FlatBright,
			0, _ImgInfo.width, _ImgInfo.height, _ImgInfo.width, nAvgBright, _pTempParam->InspArea.X1, _pTempParam->InspArea.X2);
		// Pyramid 영상 생성에 사용할 Src Img 선택
		pSrc = _pFlat->GetFlatImg();
	}
	else	// Pyramid 영상 생성에 사용할 Src Img 선택
		pSrc = _ImgInfo.pImg;

	clock_t time5 = clock();
	strTemp.Format(L"Elapsed Time 4 : %0.3f", double(time5 - time4) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	// 피라미드 영상을 생성
	if (_pPyramid->IsFinish() == false)
		_pPyramid->MakeImage(pSrc, _ImgInfo.width, _ImgInfo.height);

	clock_t time6 = clock();
	strTemp.Format(L"Elapsed Time 5 : %0.3f", double(time6 - time5) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	// 스크래치 검사의 경우에는 원본 영상을 사용한다.
	// 평활화 하면서 수직 스크래치 정보가 손실될 수 있음.
	if (pParam->_pSC != nullptr)
		_pPyramidSc->MakeImage(pSrc, _ImgInfo.width, _ImgInfo.height);

	// 검사 진행
	_pInspect->ResetDefectData();
	_pInspect->Run();

	clock_t time7 = clock();
	strTemp.Format(L"Elapsed Time 6 : %0.3f", double(time7 - time6) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	strTemp.Format(L"Elapsed Time  : %0.3f", double(time7 - time1) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	// 검사 결과 처리
	DEFECTDATA* pData = _pInspect->GetDefectData();

	strTemp.Format(L"Process Num : %d", _ProcIdx);
	OutputDebugString(strTemp);

	strTemp.Format(L"Defect Count : %d", pData->Count);
	OutputDebugString(strTemp);

	for (int i = 0; i < pData->Count; i++)
	{
		DEFECT* pDefect = &pData->pInfo[i];

		// X-offset 발생한 거리만큼 실제 거리 보정하여 처리함. 
		pDefect->x_pos += _pTempParam->CropInfo.StX;
		strTemp.Format(L"DefectInfo_%d : %0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %d", i, pDefect->x_pos, pDefect->y_pos, pDefect->sizeX, pDefect->sizeY, pDefect->size, pDefect->type);
		OutputDebugString(strTemp);
	}
}

