#include "Inspector.h"

#include "atlimage.h"

Inspector::Inspector()
{
	_sysParam.Load(L"C:\\COSS\\");
	_pParam = nullptr;
	
	_pSrcImg = nullptr;
	_SrcW = _SrcH = 0;

	_NumOfProc = 0;
	_pInspect = nullptr;
	_tempParam = nullptr;
	_pMarkParam = nullptr;

	/////////////////////////////////////////////
	// 전처리 클래스
	_pFlat			= nullptr;	
	_pProfile		= nullptr;
	_pEdgeFinder	= nullptr;
	_pPyramid		= nullptr;
	_pPyramidSc		= nullptr;
	// 전처리 클래스
	/////////////////////////////////////////////
}

Inspector::~Inspector()
{
	Release();
}

void Inspector::Initialize(int numOfProc)
{
	Release();

	_NumOfProc = numOfProc;

	_pInspect = new Inspect* [numOfProc];
	_pParam = new Param* [numOfProc];

	_tempParam = new TempParam::TEMP_PARAM[numOfProc];

	_pMarkParam = new MARK_OPTIC[numOfProc];

	for (int i = 0; i < numOfProc; i++)
	{
		_pInspect[i] = nullptr;
		_pParam[i] = nullptr;
	}
}

void Inspector::SetParam(int procNum, std::wstring pcName, std::wstring paramPath)
{
	TempParam::TEMP_PARAM* pTmpParam = &_tempParam[procNum];
	
	pTmpParam->PcInfo.Set(pcName);

	std::wstring section = pTmpParam->PcInfo.Name + std::wstring(L" - Inspect Parameter");
	eParamType::eType type = Param::GetParamType(paramPath, section);

	if (_pParam[procNum] != nullptr)
	{
		delete _pParam[procNum];
		_pParam[procNum] = nullptr;
	}

	switch (type)
	{
	case eParamType::eCOS1 :	
		_pParam[procNum] = new Cos1Param();
		_pInspect[procNum] = new InspectCOS1();
		break;
	case eParamType::eCOS2 :	
		_pParam[procNum] = new Cos2Param();
		_pInspect[procNum] = new InspectCOS2();
		break;
	case eParamType::eCOS3 :	
		_pParam[procNum] = new Cos3Param();
		_pInspect[procNum] = new InspectCOS3();
		break;
	case eParamType::eCOS4 :	
		_pParam[procNum] = new Cos4Param();
		_pInspect[procNum] = new InspectCOS4();
		break;
	case eParamType::eCOS5 :	
		_pParam[procNum] = new Cos5Param();
		_pInspect[procNum] = new InspectCOS5();
		break;
	case eParamType::eCOS6 :	
		_pParam[procNum] = new Cos6Param();
		_pInspect[procNum] = new InspectCOS6();
		break;
	case eParamType::eCOS7 :	
		_pParam[procNum] = new Cos7Param();
		_pInspect[procNum] = new InspectCOS7();
		break;
	case eParamType::eCOS8 :	
		_pParam[procNum] = new Cos8Param();
		_pInspect[procNum] = new InspectCOS8();
		break;
	case eParamType::eCOS9 :	
		_pParam[procNum] = new Cos9Param();
		_pInspect[procNum] = new InspectCOS9();
		break;
	case eParamType::eCOSA :	
		_pParam[procNum] = new CosAParam();
		_pInspect[procNum] = new InspectCOSA();
		break;
	case eParamType::eCOSB :	
		_pParam[procNum] = new CosBParam();
		_pInspect[procNum] = new InspectCOSB();
		break;
	case eParamType::eCOSC :	
		_pParam[procNum] = new CosCParam();
		_pInspect[procNum] = new InspectCOSC();
		break;
	case eParamType::eCOSD :	
		_pParam[procNum] = new CosDParam();
		_pInspect[procNum] = new InspectCOSD();
		break;
	case eParamType::eCOSE :	
		_pParam[procNum] = new CosEParam();
		_pInspect[procNum] = new InspectCOSE();
		break;
	case eParamType::eCBCR :	
		_pParam[procNum] = new CBCRParam();
		_pInspect[procNum] = new InspectCOSBCR();
		break;
	}

	Inspect* pIncp = _pInspect[procNum];
	Param* pParam = _pParam[procNum];

	pParam->Load(&pTmpParam->PcInfo, paramPath, &_sysParam);
	pIncp->Create(_sysParam.ImageW, _sysParam.ImageH);

	_pMarkParam[procNum].Load(&pTmpParam->PcInfo, paramPath);

	/////////////////////////////////////////////////////////////////////////////////
	// 평활화 이미지 클래스	 - S
	_pFlat = new FlatImage();
	_pFlat->Init(_sysParam.ImageW, _sysParam.ImageH);
	// 평활화 이미지 클래스	 - E
	/////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////
	// 프로파일 생성 클래스	 - S
	_pProfile = new Profile();
	// 프로파일 생성 클래스	 - E
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////
	// 에지 검출 클래스	 - S
	_pEdgeFinder = new FindEdge();
	_pEdgeFinder->Init(_sysParam.ImageW, _sysParam.ImageH);
	// 에지 검출 클래스	 - E
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////
	// 피라미드 이미지 클래스	- S
	// Spot 검사용 
	_pPyramid = new PyramidImage();
	_pPyramid->SetImageInfo(_sysParam.ImageW, _sysParam.ImageH);

	// 스크래치 검사용 이미지
	_pPyramidSc = new PyramidImage(2);	// 2배 축소한 이미지만 필요함. 
	_pPyramidSc->SetImageInfo(_sysParam.ImageW, _sysParam.ImageH);
	// 피라미드 이미지 클래스	- E
	/////////////////////////////////////////////////////////////////////////////////

	_SrcW = _sysParam.ImageW;
	_SrcH = _sysParam.ImageH;
	_pSrcImg = new unsigned char[_SrcW * _SrcH];
	//memset(_pSrcImg, 0x00, sizeof(unsigned char)* _SrcW* _SrcH);

	//////////////////////////////////////////////////////////////////////////////////
	// 각 전처리 클래스 및 데이터 클래스를 검사 클래스로 연결	- S
	pIncp->SetSrcImage(_pSrcImg);
	pIncp->SetFlatCls(_pFlat);
	pIncp->SetEdgeFinderCls(_pEdgeFinder);
	pIncp->SetProfileCls(_pProfile);
	pIncp->SetPyramidCls(_pPyramid, 0);

	pIncp->SetSysParam(&_sysParam);
	pIncp->SetTempParam(&pTmpParam[procNum]);
	pIncp->SetParam(_pParam[procNum]);
	pIncp->SetMarkParam(&_pMarkParam[procNum]);

	if (pParam->_pSC != nullptr)
	{
		pIncp->SetPyramidCls(_pPyramidSc, 1);
		pIncp->SetScInspBuf(_SrcW, _SrcH);
	}
	// 각 전처리 클래스 및 데이터 클래스를 검사 클래스로 연결	- E
	//////////////////////////////////////////////////////////////////////////////////
}

void Inspector::Release()
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

	if (_pEdgeFinder != nullptr)
	{
		delete _pEdgeFinder;
		_pEdgeFinder = nullptr;
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

	if (_pMarkParam != nullptr)
	{
		delete _pMarkParam;
		_pMarkParam = nullptr;
	}

	if (_pInspect != nullptr)
	{
		for (int i = 0; i < _NumOfProc; i++)
		{
			if (_pInspect[i] != nullptr)
			{
				delete _pInspect[i];
				_pInspect[i] = nullptr;
			}
		}
		delete[] _pInspect;
		_pInspect = nullptr;
	}

	if (_pSrcImg != nullptr)
	{
		delete _pSrcImg;
		_pSrcImg = nullptr;
	}
}

void Inspector::SetImage(unsigned char* pBuf, int width, int height, int pitch)
{
	if (width == pitch)
		memcpy(_pSrcImg, pBuf, sizeof(unsigned char) * width * height);
	else
	{
		for (int i = 0; i < height; i++)
			memcpy(_pSrcImg + i * width, pBuf + i * pitch, sizeof(unsigned char) * width);
	}
}

void Inspector::RunWebInspect(int procNum)
{
	unsigned char* pSrc;

	Param* pParam = _pParam[procNum];
	CString strTemp;

	clock_t start = clock();

	_pPyramid->Reset();

	/*int start = GetTickCount();*/

	clock_t time1 = clock();
	_pProfile->MakeProfileData(_pSrcImg, _SrcW, _SrcH, _SrcW);
	clock_t time2 = clock();
	strTemp.Format(L"Elapsed Time 1 : %0.3f", double(time2 - time1) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	_pEdgeFinder->GetEdge(&_tempParam[procNum].InspArea, _pSrcImg, _SrcW, _SrcH, pParam->Cam.ScaleX, pParam->Common.EdgeTh, pParam->Common.AlgorithmType, pParam->Common.EdgeOffset, pParam->Common.EdgeDir, pParam->Common.NotInspArea);

	clock_t time3 = clock();
	strTemp.Format(L"Elapsed Time 2 : %0.3f", double(time3 - time2) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	int nAvgBright = _pProfile->CalcAvgBright(_pEdgeFinder->m_nInspX1, _pEdgeFinder->m_nInspX2, _sysParam.FlatBright);

	clock_t time4 = clock();
	strTemp.Format(L"Elapsed Time 3 : %0.3f", double(time4 - time3) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);
	
	if (pParam->Common.useFlatImage)
	{
		_pFlat->MakeFlatLineScan(_pSrcImg, _pProfile->GetProfile(), _sysParam.FlatBright, 0, _SrcW, _SrcH, _SrcW, nAvgBright, _tempParam[procNum].InspArea.X1, _tempParam[procNum].InspArea.X2);
		// Pyramid 영상 생성에 사용할 Src Img 선택
		pSrc = _pFlat->GetFlatImg();
	}
	else	// Pyramid 영상 생성에 사용할 Src Img 선택
		pSrc = _pSrcImg;

	clock_t time5 = clock();
	strTemp.Format(L"Elapsed Time 4 : %0.3f", double(time5 - time4) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	// 피라미드 영상을 생성
	if (_pPyramid->IsFinish() == false)
		_pPyramid->MakeImage(pSrc, _SrcW, _SrcH);

	clock_t time6 = clock();
	strTemp.Format(L"Elapsed Time 5 : %0.3f", double(time6 - time5) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	// 스크래치 검사의 경우에는 원본 영상을 사용한다.
	// 평활화 하면서 수직 스크래치 정보가 손실될 수 있음.
	if (pParam->_pSC != nullptr)
		_pPyramidSc->MakeImage(_pSrcImg, _SrcW, _SrcH);


	
	

	//CImage tmpImage;
	//RGBQUAD   bmiColors[256];
	//for (int i = 0; i < 256; i++)
	//{
	//	bmiColors[i].rgbRed = bmiColors[i].rgbGreen = bmiColors[i].rgbBlue = (BYTE)i;
	//	bmiColors[i].rgbReserved = 0;
	//}

	//for (int i = 0; i < 5; i++)
	//{
	//	tmpImage.Create(_pPyramid->GetImageWidth(i), _pPyramid->GetImageHeight(i), 8);
	//	tmpImage.SetColorTable(0, 256, bmiColors);


	//	LPBYTE dst = (LPBYTE)tmpImage.GetBits();

	//	LPBYTE src = _pPyramid->GetImagePt(i);
	//	int y, pitch1 = tmpImage.GetPitch();
	//	for (y = 0; y < _pPyramid->GetImageHeight(i); y++, src += _pPyramid->GetImageWidth(i), dst += pitch1)
	//		CopyMemory(dst, src, _pPyramid->GetImageWidth(i));

	//	std::wstring fileName;
	//	fileName.append(L"d:\\test_");
	//	fileName.append(std::to_wstring(i));
	//	fileName.append(L".bmp");
	//	tmpImage.Save(fileName.c_str(), Gdiplus::ImageFormatBMP);

	//	tmpImage.Destroy();
	//}
	

	// 검사 진행
	_pInspect[procNum]->ResetDefectData();
	_pInspect[procNum]->Run();

	clock_t time7 = clock();
	strTemp.Format(L"Elapsed Time 6 : %0.3f", double(time7 - time6) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	strTemp.Format(L"Elapsed Time  : %0.3f", double(time7 - time1) * 1000.0 / CLOCKS_PER_SEC);
	OutputDebugString(strTemp);

	// 검사 결과 처리


	DEFECTDATA* pData = _pInspect[procNum]->GetDefectData();

	CImage tmpImage;
	RGBQUAD   bmiColors[256];
	for (int i = 0; i < 256; i++)
	{
		bmiColors[i].rgbRed = bmiColors[i].rgbGreen = bmiColors[i].rgbBlue = (BYTE)i;
		bmiColors[i].rgbReserved = 0;
	}

	tmpImage.Create(BAD_IMG_WIDTH, BAD_IMG_HEIGHT, 8);
	tmpImage.SetColorTable(0, 256, bmiColors);


	for (int i = 0; i < pData->Count; i++)
	{
		LPBYTE dst = (LPBYTE)tmpImage.GetBits();

		LPBYTE src = pData->pImage[i];
		int y, pitch1 = tmpImage.GetPitch();
		for (y = 0; y < BAD_IMG_HEIGHT; y++, src += BAD_IMG_WIDTH, dst += pitch1)
			CopyMemory(dst, src, BAD_IMG_WIDTH);

		std::wstring fileName;
		fileName.append(L"c:\\COSS\\LOTDATA\\TEST\\test_");
		fileName.append(std::to_wstring(i));
		fileName.append(L".bmp");
		tmpImage.Save(fileName.c_str(), Gdiplus::ImageFormatBMP);
	}

	tmpImage.Destroy();
}
