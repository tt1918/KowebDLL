#include "Inspector.h"

Inspector::Inspector()
{
	_sysParam.Load("C:\\COSS\\");
	_pParam = nullptr;
	
	_pSrcImg = nullptr;
	_SrcW = _SrcH = 0;

	_pInspect = nullptr;
	_tempParam.Reset();

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

}

void Inspector::Initialize()
{

}

void Inspector::Initialize(std::string paramPath)
{
	std::string section = _tempParam.PcInfo.Name + std::string(" - Inspect Parameter");
	eParamType::eType type = Param::GetParamType(paramPath, section);

	if (_pParam != nullptr)
	{
		delete _pParam;
		_pParam = nullptr;
	}

	switch (type)
	{
	case eParamType::eCOS1 :	
		_pParam = new Cos1Param(); 
		_pInspect = new InspectCOS1();
		break;
	case eParamType::eCOS2 :	
		_pParam = new Cos2Param();
		_pInspect = new InspectCOS2();
		break;
	case eParamType::eCOS3 :	
		_pParam = new Cos3Param();
		_pInspect = new InspectCOS3();
		break;
	case eParamType::eCOS4 :	
		_pParam = new Cos4Param();
		_pInspect = new InspectCOS4();
		break;
	case eParamType::eCOS5 :	
		_pParam = new Cos5Param();
		_pInspect = new InspectCOS5();
		break;
	case eParamType::eCOS6 :	
		_pParam = new Cos6Param();
		_pInspect = new InspectCOS6();
		break;
	case eParamType::eCOS7 :	
		_pParam = new Cos7Param();
		_pInspect = new InspectCOS7();
		break;
	case eParamType::eCOS8 :	
		_pParam = new Cos8Param();
		_pInspect = new InspectCOS8();
		break;
	case eParamType::eCOS9 :	
		_pParam = new Cos9Param();
		_pInspect = new InspectCOS9();
		break;
	case eParamType::eCOSA :	
		_pParam = new CosAParam();
		_pInspect = new InspectCOSA();
		break;
	case eParamType::eCOSB :	
		_pParam = new CosBParam();
		_pInspect = new InspectCOSB();
		break;
	case eParamType::eCOSC :	
		_pParam = new CosCParam();
		_pInspect = new InspectCOSC();
		break;
	case eParamType::eCOSD :	
		_pParam = new CosDParam();
		_pInspect = new InspectCOSD();
		break;
	case eParamType::eCOSE :	
		_pParam = new CosEParam();
		_pInspect = new InspectCOSE();
		break;
	case eParamType::eCBCR :	
		_pParam = new CBCRParam();
		_pInspect = new InspectCOSBCR();
		break;
	}

	_pParam->Load(paramPath);
	_pInspect->Create(_sysParam.ImageW, _sysParam.ImageH);


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
	memset(_pSrcImg, 0x00, sizeof(unsigned char)* _SrcW* _SrcH);

	//////////////////////////////////////////////////////////////////////////////////
	// 각 전처리 클래스 및 데이터 클래스를 검사 클래스로 연결	- S
	_pInspect->SetSrcImage(_pSrcImg);
	_pInspect->SetFlatCls(_pFlat);
	_pInspect->SetEdgeFinderCls(_pEdgeFinder);
	_pInspect->SetProfileCls(_pProfile);
	_pInspect->SetPyramidCls(_pPyramid, 0);

	_pInspect->SetSysParam(&_sysParam);
	_pInspect->SetTempParam(&_tempParam);
	_pInspect->SetParam(_pParam);

	if (_pParam->_pSC != nullptr)
	{
		_pInspect->SetPyramidCls(_pPyramidSc, 1);
		_pInspect->SetScInspBuf(_SrcW, _SrcH);
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

	if (_pInspect != nullptr)
	{
		delete _pInspect;
		_pInspect = nullptr;
	}

	if (_pSrcImg != nullptr)
	{
		delete _pSrcImg;
		_pSrcImg = nullptr;
	}
}

void Inspector::SetImage(unsigned char* pBuf, int width, int pitch, int height)
{
	if (width == pitch)
		memcpy(_pSrcImg, pBuf, sizeof(unsigned char) * width * height);
	else
	{
		for (int i = 0; i < height; i++)
			memcpy(_pSrcImg + i * width, pBuf + i * pitch, sizeof(unsigned char) * width);
	}
}

void Inspector::RunWebInspect()
{
	unsigned char* pSrc;

	_pProfile->MakeProfileData(_pSrcImg, _SrcW, _SrcH, _SrcW);
	int nAvgBright = _pProfile->CalcAvgBright(_pEdgeFinder->m_nInspX1, _pEdgeFinder->m_nInspX2, _sysParam.FlatBright);

	_pEdgeFinder->GetEdge(&_tempParam.InspArea, _pSrcImg, _SrcW, _SrcH, _pParam->Cam.ScaleX, _pParam->Common.EdgeTh, _pParam->Common.AlgorithmType, _pParam->Common.EdgeOffset, _pParam->Common.EdgeDir, _pParam->Common.NotInspArea);
	
	if(_pParam->Common.useFlatImage)
		_pFlat->MakeFlatLineScan(_pSrcImg, _pFlat->GetFlatImg(), _pProfile->GetProfile(), _sysParam.FlatBright, 0, _SrcW, _SrcH, _SrcW, nAvgBright, _tempParam.InspArea.X1, _tempParam.InspArea.X2);

	// Pyramid 영상 생성에 사용할 Src Img 선택
	if (_pParam->Common.useFlatImage)	pSrc = _pFlat->GetFlatImg();
	else								pSrc = _pSrcImg;

	// 피라미드 영상을 생성
	if (_pPyramid->IsFinish() == false)
		_pPyramid->MakeImage(pSrc, _SrcW, _SrcH);

	// 검사 진행
	_pInspect->Run();

	// 검사 결과 처리
}
