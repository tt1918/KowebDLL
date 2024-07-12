#include "Inspect.h"
#include <ppl.h>

Inspect::Inspect()
{
	Init();
}


Inspect::~Inspect()
{
}

void Inspect::Init()
{
	// 참조 메모리
	_pParam = nullptr;
	_pTmpData = nullptr;
	_SrcImg = nullptr;
	_pMark = nullptr;

	// 할당 메모리
	_pFlat = nullptr;	// 클래스
	_pProfile = nullptr;
	_pEdgeFinder = nullptr;
	_pPyramid = nullptr;
	_pPyramidSc = nullptr;
	_pChain = nullptr;

	_pFlatImg = nullptr;	// 메모리

	_width = _height = 0;

	_pKProjFlatAvg = nullptr;
	_pKProj = nullptr;
	_pKProjLocal = nullptr;
	_pKProjLocalArray = nullptr;
	_pKProjErode = nullptr;
	_pKProjDilate = nullptr;
	_pKProjFlat = nullptr;
	_pKProjFlat1 = nullptr;
	_pKProjFlatMax = nullptr;

	_pFmTemp = nullptr;
}

void Inspect::Create(int width, int height)
{
	Release();

	_pFlat = new FlatImage();
	_pProfile = new Profile();
	_pEdgeFinder = new FindEdge();
	_pPyramid = new PyramidImage();
	_pPyramidSc = new PyramidImage(2);	// 2배 축소한 이미지만 필요함. 
	_pPyramid->SetImageInfo(width, height);

	// 갯수는 임의 할당.. 나중에 보고 처리해야함.
	_pChain = new CChain(1280, 20000);

	_pFlatImg = new BYTE[width * height];
	memset(_pFlatImg, 0x00, sizeof(BYTE) * width * height);

	_pFmTemp = new BYTE[width * height];
	memset(_pFmTemp, 0x00, sizeof(BYTE) * width * height);

	SetScInspBuf(width, height);
}

void Inspect::Release()
{
	if (_pFlat)
	{
		delete _pFlat;
		_pFlat = nullptr;
	}

	if (_pProfile)
	{
		delete _pProfile;
		_pProfile = nullptr;
	}

	if (_pEdgeFinder)
	{
		delete _pEdgeFinder;
		_pEdgeFinder = nullptr;
	}

	if (_pPyramid)
	{
		delete _pPyramid;
		_pPyramid = nullptr;
	}

	if (_pPyramidSc)
	{
		delete _pPyramidSc;
		_pPyramidSc = nullptr;
	}

	if (_pFlatImg)
	{
		delete[] _pFlatImg;
		_pFlatImg = nullptr;
	}

	if (_pChain)
	{
		delete _pChain;
		_pChain = nullptr;
	}

	_width = _height = 0;

	if (_pFmTemp)
	{
		delete[] _pFmTemp;
		_pFmTemp = nullptr;
	}

	if (_pKProjFlatAvg != nullptr)
	{
		delete[] _pKProjFlatAvg;
		_pKProjFlatAvg = nullptr;
	}

	if (_pKProj != nullptr)
	{
		delete[] _pKProj;
		_pKProj = nullptr;
	}

	if (_pKProjLocal != nullptr)
	{
		delete[] _pKProjLocal;
		_pKProjLocal = nullptr;
	}

	if (_pKProjLocalArray != nullptr)
	{
		delete[] _pKProjLocalArray;
		_pKProjLocalArray = nullptr;
	}

	if (_pKProjErode != nullptr)
	{
		delete[] _pKProjErode;
		_pKProjErode = nullptr;
	}

	if (_pKProjDilate != nullptr)
	{
		delete[] _pKProjDilate;
		_pKProjDilate = nullptr;
	}

	if (_pKProjFlat != nullptr)
	{
		delete[] _pKProjFlat;
		_pKProjFlat = nullptr;
	}

	if (_pKProjFlat1 != nullptr)
	{
		delete[] _pKProjFlat1;
		_pKProjFlat1 = nullptr;
	}

	if (_pKProjFlatMax != nullptr)
	{
		delete[] _pKProjFlatMax;
		_pKProjFlatMax = nullptr;
	}
}

void Inspect::SetParam(Param* pParam) 
{ 
	_pParam = pParam; 
}

void Inspect::SetTempParam(TempParam::TEMP_PARAM* pTmpParam) 
{ 
	_pTmpData = pTmpParam; 
}

void Inspect::Run()
{

}

void Inspect::MakePyramid()
{
	unsigned char* pSrc;

	if (_pParam->Common.useFlatImage)	pSrc = _pFlatImg;
	else								pSrc = _SrcImg;

	// 피라미드 영상을 생성
	if (_pTmpData->MakePyramidDone == 0)
	{
		_pPyramid->MakeImage(pSrc, _width, _height);
		_pTmpData->MakePyramidDone = 1;
	}
}

void Inspect::SetScInspBuf(int width, int height)
{
	_pKProjFlatAvg = new long[width];
	_pKProj = new long[width];
	_pKProjLocal = new long[width];

	_pKProjLocalArray = new int[width * (height/16)];
	_pKProjErode = new long[width];
	_pKProjDilate = new long[width];

	_pKProjFlat = new long[width];
	_pKProjFlat1 = new long[width];
	_pKProjFlatMax = new long[width];
}

void Inspect::SpotInspect()
{
	if (_pParam == nullptr) return;

	int i, nOverlap;
	int nLevel, nAvg, nValueUp, nValueDn, nValueUp1, nValueDn1, nValueUpBig, nValueDnBig;
	double dSize[5], dSize1[5], dSizeBig[5];
	int nPLevel, nPValueUp, nPValueDn, nPAvg;	//찍힘검사
	int nClass, nClass_;
	double dPSize[5], dImulKipoSize, dImulKipoSize1, dImulKipoSizeBig;
	int nWhiteInspect = 0, nBlackInspect = 0, nCunicInspect = 0;
	int nSkip, leftBig, topBig, nDefectPosX, nDefectPosY, nDefectBigPosX, nDefectBigPosY;
	int nFoundLami = 0;
	int nOverkill = 0, nBigDefect;

	RECT rcTmpDefect, rcTmpDefect2;

	_pTmpData->MaxGray = 0;
	_pTmpData->MinGray = 255;

	int nArThUp = _pParam->_pLvUp->AreaTh;
	int nArThDn = _pParam->_pLvDn->AreaTh;

	InspParam::SPOT_PARAM* pLvUp = _pParam->_pLvUp;
	InspParam::SPOT_PARAM* pLvDn = _pParam->_pLvDn;
	InspParam::CUNIC_PARAM* pCunic = _pParam->_pCunic;
	InspParam::PRESS_PARAM* pPress = _pParam->_pPress;

	// 백/흑/쿠닉 파라미터 체크
	for (i = 0; i < 10; i++)
	{
		if (_pParam->UseLv[i] == true)
		{
			if (pLvUp != nullptr && (pLvUp->LvTH[i] > 0 || pLvUp->LvSize[i] > 0.0))	nWhiteInspect = 1;
			if (pLvDn != nullptr && (pLvDn->LvTH[i] > 0 || pLvDn->LvSize[i] > 0.0))	nBlackInspect = 1;
			if (_pSystem->CamType == 1 && pCunic != nullptr && (pCunic->Std[i] > 0.0 || pCunic->Std1[i] > 0.0)) nCunicInspect = 1;
		}
	}

	// 찍힘 검사가 있으면 체크
	if (_pParam->_pPress != nullptr && _pParam->_pPress->IsInsp == true) 
		nWhiteInspect = nBlackInspect = 1;

	// 찍힘 검사 파라미터 체크 
	if (pPress != nullptr)
	{
		for (i = 0; i < 10; i++)
		{
			if (_pParam->UseLv[i] == true && (pPress->Value[i] > 0 || pPress->Size[i] > 0.001))
			{
				nWhiteInspect = nBlackInspect = 1;
				break;
			}
		}
	}

	// 조명 제어 중에는 쿠닉 검사를 하지 않는다. 
	if (_pTmpData->AutoAdjStart > 0) nCunicInspect = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// NxN구간으로 나눠서 후보 찾기 - S
	int nCol, nRow;
	nCol = _pSystem->ImageW / _pSystem->CandiRect; if (_pSystem->ImageW - nCol * _pSystem->CandiRect) nCol++;
	nRow = _pSystem->ImageH / _pSystem->CandiRect; if (_pSystem->ImageH - nRow * _pSystem->CandiRect) nRow++;
	FindCandidateParallel(0, _pSystem->ImageW, _pSystem->CandiRect);
	// NxN구간으로 나눠서 후보 찾기 - E
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//큰 순서로 소팅 각 항목별 MAX_DEFECT (30개만,  흑10  백10  찍힘10)-----
	FindCandiSorting(0, _pSystem->ImageW, _pSystem->CandiRect);

	//(1/2)영상에서 MAX/Min 10개 표시---------------------------------------------------
	//백후보든 흑후보든 찍힘도 같이 검사한다.
	int nLoop = _pSystem->CandiRect;
	int nVal;
	for (i = 0; i < nLoop; i++)
	{
		// 핀홀 관련 전역 변수 초기화
		_pTmpData->Pinhole.IsLarge = false;
		_pTmpData->Pinhole.IsShadeSkip = false;
		_pTmpData->Pinhole.IsGroup = false;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 백점 검사	- S
		nVal = _CandiW.Value[i] - _CandiW.Avg[i];	if (nVal > _pTmpData->MaxGray) _pTmpData->MaxGray = nVal;
		if (nWhiteInspect && nVal >= CANDI_WHITE_LOW)
		{
			nSkip = 0;
			// 경계 투과(반사) 면 이미지가 울렁거려 단순울렁인지 Defect인지 한번 더 판단한다.
			if (_pSystem->BDOverkill && CheckBoundaryOpticOverKill(_pFlatImg, _CandiW.PosX[i], _CandiW.PosY[i], _pSystem->ImageW, _pSystem->ImageH, 13)) nSkip = 1;
			if (nSkip == 0 && _pParam->Common.AlgorithmType == COS2 && CheckSimpleGradient(_CandiW.PosX[i], _CandiW.PosY[i], 0, _pSystem->ImageW, _pSystem->ImageH, 3.5))	nSkip = 1;
			if (_CandiB.PosX[i]< _pTmpData->InspArea.X1 || _CandiB.PosX[i]>_pTmpData->InspArea.X2)	nSkip = 1;
		
			if (nSkip == 0)
			{
				dSize1[0] = dSize1[1] = dSize1[2] = dImulKipoSize1 = nValueUp1 = nValueDn1 = 0;
				MakeDefectRect(_DefectData.Count, _CandiW.PosX[i], _CandiW.PosY[i], _pTmpData->InspArea.X1, _pTmpData->InspArea.X2);
				GetSizeNValue(0, _pFlatImg, _CandiW.PosY[i], _DefectData.Area[_DefectData.Count].left, _DefectData.Area[_DefectData.Count].top, BAD_IMG_WIDTH, BAD_IMG_WIDTH, _pSystem->ImageW, 0,
					_pParam->_pPress->AreaUpTH, _pParam->_pPress->AreaDnTH, _pParam->Common.AttachPxl, _pParam->Common.SizeMethod, _pParam->Cam.ScaleX, _pParam->Cam.ScaleY,
					&nAvg, &nValueUp, &nValueDn, dSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY, &rcTmpDefect);

				// 4배 압축 영상에서 Size 구함
				nBigDefect = 0;
				if (_pSystem->UseBigDefect == true && (dSize[0] >= (BAD_IMG_WIDTH * _pParam->Cam.ScaleX) / 2.0 || dSize[1] >= (BAD_IMG_HEIGHT * _pParam->Cam.ScaleY) / 2.0))
				{
					MakeDefectRect_BigDefect(_CandiW.PosX[i], _CandiW.PosY[i], _pTmpData->InspArea.X1, _pTmpData->InspArea.X2, &leftBig, &topBig);
					GetSizeNValue(0, _pPyramid->GetImagePt(2), _CandiW.PosY[i] / 4, leftBig, topBig, BAD_IMG_WIDTH, BAD_IMG_WIDTH, _pSystem->ImageW / 4, 0,
						_pParam->_pLvUp->AreaTh, _pParam->_pLvDn->AreaTh, 0, _pParam->Common.SizeMethod, _pParam->Cam.ScaleX * 4, _pParam->Cam.ScaleY * 4,
						&nAvg, &nValueUpBig, &nValueDnBig, dSizeBig, &dImulKipoSizeBig, &nDefectBigPosX, &nDefectBigPosY, &rcTmpDefect);

					if (dSizeBig[2] > dSize[2])
					{
						nBigDefect = 1;
						dSize[0] = dSizeBig[0];
						dSize[1] = dSizeBig[1];
						dSize[2] = dSizeBig[2];
					}
				}

				if (nValueUp > _pTmpData->MaxGray)	_pTmpData->MaxGray = nValueUp;
				nLevel = GetLevel(nValueUp, dSize[2], nAvg, _pParam->_pLvUp->LvTH, _pParam->_pLvUp->LvSize);

				nClass = nClass_ = nLevel * MILLION + _pTmpData->PcInfo.FirstNo * CLASSDEV + g_NGType._SpotW;  //백점

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// 찍힘검사 설정되어 있으면 찍힘검사한다. - S
				if (_pParam->_pPress != nullptr && _pParam->_pPress->IsInsp == true)
				{
					GetSizeNValue(2, _pFlatImg, _CandiW.PosY[i], _DefectData.Area[_DefectData.Count].left, _DefectData.Area[_DefectData.Count].top, BAD_IMG_WIDTH, BAD_IMG_WIDTH, _pSystem->ImageW, 0,
						_pParam->_pPress->AreaUpTH, _pParam->_pPress->AreaDnTH, _pParam->Common.AttachPxl, _pParam->Common.SizeMethod, _pParam->Cam.ScaleX, _pParam->Cam.ScaleY,
						&nPAvg, &nPValueUp, &nPValueDn, dPSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY, &rcTmpDefect2);
					if (nPValueUp >= _pParam->_pPress->UpTh && nPValueDn >= _pParam->_pPress->DnTh)
						nPLevel = GetLevel(nPValueUp + nPValueDn, dPSize[2], nPAvg, _pParam->_pPress->Value, _pParam->_pPress->Size);
					else
						nPLevel = 0;

					if (nPLevel > 0)
					{
						nClass_ = SelectDefect(nPLevel, g_NGType._SpotPress, nLevel, nClass % CLASSDEV); //백점과 찍힘중에 선택한다. 
						if (nClass_ % CLASSDEV == g_NGType._SpotPress)
						{
							nValueUp = nPValueUp;
							nValueDn = nPValueDn;
							rcTmpDefect = rcTmpDefect2;
						}
					}
				}
				// 찍힘검사 설정되어 있으면 찍힘검사한다. - E
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				if (nClass > MILLION || nClass_ > MILLION)
				{
					_DefectData.FrameNum = _pTmpData->GrabFrameID;
					_DefectData.Info[_DefectData.Count].x_pos = (float)_CandiW.PosX[i];			//불량위치 X (Pixel)
					_DefectData.Info[_DefectData.Count].y_pos = (float)_CandiW.PosY[i];			//불량위치 Y (Pixel)

					if (nClass == nClass_) //(백점 또는 흑점)이라는 뜻
					{
						if (_pSystem->UseBigDefect && nBigDefect)
						{
							_DefectData.Info[_DefectData.Count].x_pos = (float)(nDefectBigPosX * 4);
							_DefectData.Info[_DefectData.Count].y_pos = (float)(nDefectBigPosY * 4);
							MakeDefectRect_BigDefect1(_DefectData.Count, nDefectBigPosX * 4, nDefectBigPosY * 4, _pTmpData->InspArea.X1, _pTmpData->InspArea.X2);
							MakeDefectRect_BigDefect(nDefectBigPosX * 4, nDefectBigPosY * 4, _pTmpData->InspArea.X1, _pTmpData->InspArea.X2, &leftBig, &topBig);
							CopyNGImageFMArea(_pPyramid->GetImagePt(2), leftBig, topBig, leftBig + BAD_IMG_WIDTH, topBig + BAD_IMG_WIDTH, _pSystem->ImageW/ 4, 255);
						}
						else
						{
							if (_pParam->Common.AlgorithmType != COS4)	CopyNGImage(true);
							else										CopyNGImage(false);
						}
			
						_DefectData.Info[_DefectData.Count].type = nClass;				//불량TYPE
						_DefectData.Info[_DefectData.Count].size = (float)dSize[2];			//Size	(넥스트아이 서버와 맞추기 위해)


						if (nClass % CLASSDEV == g_NGType._SpotW || nClass % CLASSDEV == g_NGType._Group)
							_DefectData.Info[_DefectData.Count].value = (float)nValueUp;		//Value	(넥스트아이 서버와 맞추기 위해)		

						_DefectData.Info[_DefectData.Count].sizeX = (float)dSize[0];			//Size X
						_DefectData.Info[_DefectData.Count].sizeY = (float)dSize[1];			//Size Y
					}
					else	//찍힘이라는 뜻
					{
						if (_pParam->Common.AlgorithmType != COS4)	CopyNGImage(true);
						else										CopyNGImage(false);

						_DefectData.Info[_DefectData.Count].type = nClass_;			//불량TYPE
						_DefectData.Info[_DefectData.Count].size = (float)dPSize[2];			//Size
						_DefectData.Info[_DefectData.Count].value = (float)(nValueUp + nValueDn);	//Value					
						_DefectData.Info[_DefectData.Count].sizeX = (float)dPSize[0];			//Size X
						_DefectData.Info[_DefectData.Count].sizeY = (float)dPSize[1];			//Size Y
					}

					//겹치는 불량 점검-----------------------------------------------------------------------------------------------
					memcpy(&_DefectData.Area[_DefectData.Count], &rcTmpDefect, sizeof(RECT));
					nOverlap = CheckDefectOverlap();
					if (nOverlap == 0 && _DefectData.Count < _pSystem->MaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
					{
						_DefectData.Count++;
					}
					else
					{
						if (nOverlap == 3)		//불량수가 최대갯수 이상이면
						{
							if (_DefectData.Info[_DefectData.Count].type % CLASSDEV == g_NGType._SpotPress)  CheckPriorLevel(1);
							else																			 CheckPriorLevel(0);
						}
					}
					//---------------------------------------------------------------------------------------------------------------						
				}

			}
		}
		// 백점 검사	- E
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 흑점 검사 - S
		nVal = _CandiB.Value[i] - _CandiB.Avg[i];	if (nVal < _pTmpData->MaxGray) _pTmpData->MinGray = nVal;
		if (nBlackInspect && -nVal >= CANDI_WHITE_LOW)
		{
			nSkip = 0;

			// 경계 투과(반사) 면 이미지가 울렁거려 단순울렁인지 Defect인지 한번 더 판단한다.
			if (_pSystem->BDOverkill && CheckBoundaryOpticOverKill(_pFlatImg, _CandiB.PosX[i], _CandiB.PosY[i], _pSystem->ImageW, _pSystem->ImageH, 13)) nSkip = 1;
			if (nSkip == 0 && _pParam->Common.AlgorithmType == COS2 && CheckSimpleGradient(_CandiB.PosX[i], _CandiB.PosY[i], 0, _pSystem->ImageW, _pSystem->ImageH, 3.5))	nSkip = 1;
			if (_CandiB.PosX[i]< _pTmpData->InspArea.X1|| _CandiB.PosX[i]>_pTmpData->InspArea.X2)	nSkip = 1;

			if (nSkip == 0)
			{
				dSize1[0] = dSize1[1] = dSize1[2] = dImulKipoSize1 = nValueUp1 = nValueDn1 = 0;
				MakeDefectRect(_DefectData.Count, _CandiB.PosX[i], _CandiB.PosY[i], _pTmpData->InspArea.X1, _pTmpData->InspArea.X2);
				GetSizeNValue(1, _pFlatImg, _CandiB.PosY[i], _DefectData.Area[_DefectData.Count].left, _DefectData.Area[_DefectData.Count].top, BAD_IMG_WIDTH, BAD_IMG_WIDTH, _pSystem->ImageW, 0,
					_pParam->_pPress->AreaUpTH, _pParam->_pPress->AreaDnTH, _pParam->Common.AttachPxl, _pParam->Common.SizeMethod, _pParam->Cam.ScaleX, _pParam->Cam.ScaleY,
					&nAvg, &nValueUp, &nValueDn, dSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY, &rcTmpDefect);

				// 4배 압축 영상에서 Size 구함
				nBigDefect = 0;
				if (_pSystem->UseBigDefect == true && (dSize[0] >= (BAD_IMG_WIDTH * _pParam->Cam.ScaleX) / 2.0 || dSize[1] >= (BAD_IMG_HEIGHT * _pParam->Cam.ScaleY) / 2.0))
				{
					MakeDefectRect_BigDefect(_CandiB.PosX[i], _CandiB.PosY[i], _pTmpData->InspArea.X1, _pTmpData->InspArea.X2, &leftBig, &topBig);
					GetSizeNValue(1, _pPyramid->GetImagePt(2), _CandiB.PosY[i] / 4, leftBig, topBig, BAD_IMG_WIDTH, BAD_IMG_WIDTH, _pSystem->ImageW / 4, 0,
						_pParam->_pLvUp->AreaTh, _pParam->_pLvDn->AreaTh, 0, _pParam->Common.SizeMethod, _pParam->Cam.ScaleX * 4, _pParam->Cam.ScaleY * 4,
						&nPAvg, &nValueUpBig, &nValueDnBig, dSizeBig, &dImulKipoSizeBig, &nDefectBigPosX, &nDefectBigPosY, &rcTmpDefect);

					if (dSizeBig[2] > dSize[2])
					{
						nBigDefect = 1;
						dSize[0] = dSizeBig[0];
						dSize[1] = dSizeBig[1];
						dSize[2] = dSizeBig[2];
					}
				}

				if (-nValueDn > _pTmpData->MinGray)	_pTmpData->MinGray = -nValueUp;

				nLevel = GetLevel(nValueDn, dSize[2], nAvg, _pParam->_pLvDn->LvTH, _pParam->_pLvDn->LvSize);
				nClass = nClass_ = nLevel * MILLION + _pTmpData->PcInfo.FirstNo * CLASSDEV + g_NGType._SpotB;  //흑점


				if (nCunicInspect)   nClass = nClass_ = nLevel * MILLION + _pTmpData->PcInfo.FirstNo * CLASSDEV + g_NGType._SpotB;	//쿠닉검사할때 흑점도 검사하면 불량번호가 일반적인것과 다름. 3으로 끝남.
				else                nClass = nClass_ = nLevel * MILLION + _pTmpData->PcInfo.FirstNo * CLASSDEV + g_NGType._SpotB;	//흑점 (2로 끝남)

				
				//이물기포 Size를 구했으면
				if (dImulKipoSize > 0.01 && _pParam->_pKipo!=nullptr)
				{
					nPLevel = GetLevel(nValueDn, dImulKipoSize, nAvg, _pParam->_pKipo->Value, _pParam->_pKipo->Size);
					if (nPLevel > 0)
					{
						nClass_ = SelectDefect(nPLevel, g_NGType._SpotKipo, nLevel, g_NGType._SpotB);  //이물기포와 흑점중에 선택한다.(같은 조건이면 앞에 있는 것, 이물기포)
						if (nClass != nClass_)
							dPSize[0] = dPSize[1] = dPSize[2] = dImulKipoSize;
					}
				}
				else //이물기포와 찍힘은 같이 사용하지 않는다.
				{
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// 찍힘검사 설정되어 있으면 찍힘검사한다. - S
					if (_pParam->_pPress != nullptr && _pParam->_pPress->IsInsp == true)
					{
						GetSizeNValue(2, _pFlatImg, _CandiB.PosY[i], _DefectData.Area[_DefectData.Count].left, _DefectData.Area[_DefectData.Count].top, BAD_IMG_WIDTH, BAD_IMG_WIDTH, _pSystem->ImageW, 0,
							_pParam->_pPress->AreaUpTH, _pParam->_pPress->AreaDnTH, _pParam->Common.AttachPxl, _pParam->Common.SizeMethod, _pParam->Cam.ScaleX, _pParam->Cam.ScaleY,
							&nPAvg, &nPValueUp, &nPValueDn, dPSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY, &rcTmpDefect2);
						if (nPValueUp >= _pParam->_pPress->UpTh && nPValueDn >= _pParam->_pPress->DnTh)
							nPLevel = GetLevel(nPValueUp + nPValueDn, dPSize[2], nPAvg, _pParam->_pPress->Value, _pParam->_pPress->Size);
						else
							nPLevel = 0;

						if (nPLevel > 0)
						{
							nClass_ = SelectDefect(nPLevel, g_NGType._SpotPress, nLevel, nClass % CLASSDEV); //백점과 찍힘중에 선택한다. 
							if (nClass_ % CLASSDEV == g_NGType._SpotPress)
							{
								nValueUp = nPValueUp;
								nValueDn = nPValueDn;
								rcTmpDefect = rcTmpDefect2;
							}
						}
					}
					// 찍힘검사 설정되어 있으면 찍힘검사한다. - E
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				}
				
				if (nClass > MILLION || nClass_ > MILLION)
				{
					_DefectData.FrameNum = _pTmpData->GrabFrameID;
					_DefectData.Info[_DefectData.Count].x_pos = (float)_CandiB.PosX[i];			//불량위치 X (Pixel)
					_DefectData.Info[_DefectData.Count].y_pos = (float)_CandiB.PosY[i];			//불량위치 Y (Pixel)

					if (nClass == nClass_) //(백점 또는 흑점)이라는 뜻
					{
						if (_pSystem->UseBigDefect && nBigDefect)
						{
							_DefectData.Info[_DefectData.Count].x_pos = (float)(nDefectBigPosX * 4);
							_DefectData.Info[_DefectData.Count].y_pos = (float)(nDefectBigPosY * 4);
							MakeDefectRect_BigDefect1(_DefectData.Count, nDefectBigPosX * 4, nDefectBigPosY * 4, _pTmpData->InspArea.X1, _pTmpData->InspArea.X2);
							MakeDefectRect_BigDefect(nDefectBigPosX * 4, nDefectBigPosY * 4, _pTmpData->InspArea.X1, _pTmpData->InspArea.X2, &leftBig, &topBig);
							CopyNGImageFMArea(_pPyramid->GetImagePt(2), leftBig, topBig, leftBig + BAD_IMG_WIDTH, topBig + BAD_IMG_WIDTH, _pSystem->ImageW / 4, 255);
						}
						else
						{
							if (_pParam->Common.AlgorithmType != COS4)	CopyNGImage(true);
							else										CopyNGImage(false);
						}

						_DefectData.Info[_DefectData.Count].type = nClass;					//불량TYPE
						_DefectData.Info[_DefectData.Count].size = (float)dSize[2];			//Size	(넥스트아이 서버와 맞추기 위해)


						if (nClass % CLASSDEV == g_NGType._SpotB)
							_DefectData.Info[_DefectData.Count].value = (float)nValueDn;		//Value	(넥스트아이 서버와 맞추기 위해)		

						_DefectData.Info[_DefectData.Count].sizeX = (float)dSize[0];			//Size X
						_DefectData.Info[_DefectData.Count].sizeY = (float)dSize[1];			//Size Y
					}
					else	//찍힘이라는 뜻
					{
						if (_pParam->Common.AlgorithmType != COS4)	CopyNGImage(true);
						else										CopyNGImage(false);

						_DefectData.Info[_DefectData.Count].type = nClass_;							//불량TYPE
						_DefectData.Info[_DefectData.Count].size = (float)dPSize[2];				//Size
						_DefectData.Info[_DefectData.Count].value = (float)(nValueUp + nValueDn);	//Value					
						_DefectData.Info[_DefectData.Count].sizeX = (float)dPSize[0];				//Size X
						_DefectData.Info[_DefectData.Count].sizeY = (float)dPSize[1];				//Size Y
					}

					//겹치는 불량 점검-----------------------------------------------------------------------------------------------
					memcpy(&_DefectData.Area[_DefectData.Count], &rcTmpDefect, sizeof(RECT));
					nOverlap = CheckDefectOverlap();
					if (nOverlap == 0 && _DefectData.Count < _pSystem->MaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
					{
						_DefectData.Count++;
					}
					else
					{
						if (nOverlap == 3)		//불량수가 최대갯수 이상이면
						{
							if (_DefectData.Info[_DefectData.Count].type % CLASSDEV == g_NGType._SpotPress)  CheckPriorLevel(1);
							else																			 CheckPriorLevel(0);
						}
					}
					//---------------------------------------------------------------------------------------------------------------						
				}

			}
		}
		// 흑점 검사	- E
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// AREA(찍힘) - S
		if (_pParam->_pPress->IsInsp && _CandiArea.Value[i] >= 3)
		{
			nSkip = 0;
			//경계투과(반사) 면 이미지가 울렁거려 단순울렁인건지 Defect인지 한번더 판단한다.
			if (_pSystem->BDOverkill && CheckBoundaryOpticOverKill(_pFlatImg, _CandiArea.PosX[i], _CandiArea.PosY[i], _pSystem->ImageW, _pSystem->ImageH, 13))	nSkip = 1;
			if (nSkip == 0 && CheckSimpleGradient(_CandiArea.PosX[i], _CandiArea.PosY[i], 0, _pSystem->ImageW, _pSystem->ImageH, 3.5))							nSkip = 1;

			if (nSkip == 0) //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			{
				MakeDefectRect(_DefectData.Count, _CandiArea.PosX[i], _CandiArea.PosY[i], _pTmpData->InspArea.X1, _pTmpData->InspArea.X2);
				GetSizeNValue(2, _pFlatImg, _CandiArea.PosY[i], _DefectData.Area[_DefectData.Count].left, _DefectData.Area[_DefectData.Count].top, BAD_IMG_WIDTH, BAD_IMG_HEIGHT, _pSystem->ImageW, 0,  
					_pParam->_pPress->AreaUpTH, _pParam->_pPress->AreaDnTH, _pParam->Common.AttachPxl, _pParam->Common.SizeMethod, _pParam->Cam.ScaleX, _pParam->Cam.ScaleY,
					&nAvg, &nValueUp, &nValueDn, dSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY, &rcTmpDefect2);
				if (nValueUp >= _pParam->_pPress->UpTh && nValueDn >= _pParam->_pPress->DnTh)
				{
					nLevel = GetLevel(nValueUp + nValueDn, dSize[2], nAvg, _pParam->_pPress->Value, _pParam->_pPress->Size);
					nClass = nLevel * MILLION + _pTmpData->PcInfo.FirstNo * CLASSDEV + g_NGType._SpotPress;  //찍힘			

					if (nClass > MILLION)
					{
						if (_pParam->Common.AlgorithmType != COS4)	CopyNGImage(true);
						else										CopyNGImage(false);

						_DefectData.FrameNum = _pTmpData->GrabFrameID;
						_DefectData.Info[_DefectData.Count].x_pos = (float)_CandiArea.PosX[i];	 //불량위치 X (Pixel)
						_DefectData.Info[_DefectData.Count].y_pos = (float)_CandiArea.PosY[i];	 //불량위치 Y (Pixel)
						_DefectData.Info[_DefectData.Count].type = nClass;				 //불량TYPE
						_DefectData.Info[_DefectData.Count].size = (float)dSize[2];			 //Size
						_DefectData.Info[_DefectData.Count].value = (float)(nValueUp + nValueDn);	 //Value	
						_DefectData.Info[_DefectData.Count].sizeX = (float)dSize[0];			 //Size X
						_DefectData.Info[_DefectData.Count].sizeY = (float)dSize[1];			 //Size Y

						//겹치는 불량 점검-----------------------------------------------------------------------------------------------
						memcpy(&_DefectData.Area[_DefectData.Count], &rcTmpDefect2, sizeof(RECT));
						nOverlap = CheckDefectOverlap();
						if (nOverlap == 0 && _DefectData.Count < _pSystem->MaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
							_DefectData.Count++;
						else
							if (nOverlap == 3) CheckPriorLevel(1); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
						//---------------------------------------------------------------------------------------------------------------		
					}
				}
			}
		}
		// AREA(찍힘) - E
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}

void Inspect::ScInspect()
{
	int i, nOverlap;
	int nLevel;
	int nClass;
	int posx[MAX_SCRATCH], posy[MAX_SCRATCH], value[MAX_SCRATCH];
	int nInspect = 0, nScratchThres = 10000;
	int AverageScratch = 0, MaxScratch = 0;
	int nScratch, nOverkill;
	int nWidth, nHeight, nPitch, nInspectX1, nInspectX2, nMulti = 1;
	LPBYTE fmORG = _SrcImg;
	LPBYTE fm = _SrcImg;


	//검사하기위한 데이타가 들어 있는지 확인---------------
	for (i = 0; i < 10; i++)
	{
		if (_pParam->UseLv[i])
			if (_pParam->_pSC->ScVal[i] < nScratchThres && _pParam->_pSC->ScVal[i] >= 100)
			{
				nScratchThres = _pParam->_pSC->ScVal[i];
				nInspect = 1;
			}
	}
	if (nInspect == 0) return;
	_pTmpData->Scratch.IsInsp = true;
	//-----------------------------------------------------

	nWidth = _pSystem->ImageW; 
	nHeight = _pSystem->ImageH;
	nPitch = _pSystem->Pitch;
	nInspectX1 = _pTmpData->InspArea.X1;
	nInspectX2 = _pTmpData->InspArea.X2;

	if (_pSystem->IsMakePyramid)  //8192이면 1/2영상에서 t
	{
		fm = _pPyramidSc->GetImagePt(1);
		nWidth /= 2;
		nHeight /= 2;
		nPitch /= 2;
		nInspectX1 /= 2;
		nInspectX2 /= 2;
		nMulti = 2;
	}


	if (_pSystem->CamType)
		nScratch = FindScratch(fm, nInspectX1, 0, nInspectX2, nHeight, nPitch, 1, 35, 0, 32, 16, 10, nScratchThres, nScratchThres, nMulti, NULL, posx, posy, value, &AverageScratch, &MaxScratch);
	else
		nScratch = FindScratch(fm, nInspectX1, 0, nInspectX2, nHeight, nPitch, 1, 15, 0, 32, 16, 10, nScratchThres, nScratchThres, nMulti, NULL, posx, posy, value, &AverageScratch, &MaxScratch);

	_pTmpData->Scratch.Max = MaxScratch;
	_pTmpData->Scratch.Avg = AverageScratch;

	for (i = 0; i < nScratch; i++)
	{
		nLevel = GetLevel(value[i], _pParam->_pSC->ScVal);
		if (nLevel < 1) continue;


		//과검 체크-----------------------------------------------------------------------------------------------------------
		nOverkill = IsOverKillForScratch(fm, posx[i], posy[i], nPitch, nHeight, nScratchThres);
		if (nOverkill) continue;

		if (_pSystem->CamType)
			nOverkill = CheckRealScratch(fm, posx[i], posy[i], nPitch, nWidth, nHeight, 24);
		else
			nOverkill = CheckRealScratch(fm, posx[i], posy[i], nPitch, nWidth, nHeight, 24);
		if (nOverkill) continue;
		//--------------------------------------------------------------------------------------------------------------------

		MakeDefectRect(_DefectData.Count, posx[i] * nMulti, posy[i] * nMulti, _pTmpData->InspArea.X1, _pTmpData->InspArea.X2);

		double dSize[5], dImulKipoSize;
		int nAvg, nValueUp, nValueDn, nDefectPosX, nDefectPosY;
		RECT rcTmpDefect;
		GetSizeNValue(0, fmORG, posy[i] * nMulti, _DefectData.Area[_DefectData.Count].left, _DefectData.Area[_DefectData.Count].top, BAD_IMG_WIDTH, BAD_IMG_WIDTH, _pSystem->ImageW, 0, 10, 255,
			_pParam->Common.AttachPxl, _pParam->Common.SizeMethod, _pParam->Cam.ScaleX, _pParam->Cam.ScaleY, 
			&nAvg, &nValueUp, &nValueDn, dSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY, &rcTmpDefect);

		CopyNGImageFM(fmORG, _pSystem->Pitch);

		nClass = nLevel * MILLION + _pTmpData->PcInfo.FirstNo * CLASSDEV + g_NGType._Scratch;
		_DefectData.FrameNum = _pTmpData->GrabFrameID;
		_DefectData.Info[_DefectData.Count].x_pos = (float)posx[i] * nMulti;
		_DefectData.Info[_DefectData.Count].y_pos = (float)posy[i] * nMulti;
		_DefectData.Info[_DefectData.Count].type =	nClass;
		_DefectData.Info[_DefectData.Count].value = (float)value[i]; //Value

		_DefectData.Info[_DefectData.Count].size = (float)dSize[2]; //Size
		_DefectData.Info[_DefectData.Count].sizeX = (float)dSize[0]; //Size X
		_DefectData.Info[_DefectData.Count].sizeY = (float)dSize[1]; //Size Y


		//겹치는 불량 점검-----------------------------------------------------------------------------------------------
		nOverlap = CheckDefectOverlap();
		if (nOverlap == 0 && _DefectData.Count < _pSystem->MaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
			_DefectData.Count++;
		else
			if (nOverlap == 3) CheckPriorLevel(1); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
		//--------------------------------------------------------------------------------------------------------------
	}
}

//평균밝기 80  스크라치 90   길면 2037    짧으면(32Pixel)   604    
//                      100  길면 4100                     1209
//                      110  길면 6136                     1814   +35까지
//                      120  길면 7591                     2116
//						130  길면 7591                     2116
//						150  길면 8055                     2116
//                      255  길면 9450                     2116
int Inspect::FindScratch(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nType, int nUpperCut, int nLowerCut, int nInspY, int nJumpY, int nErode,
				int nThUp, int nThDn, int nMulti, int* pProj, int* posx, int* posy, int* value, int* AverageScratch, int* MaxScratch)
{
	int i, j, k, m;
	int nScratchCount = 0;
	int nProjCount = 256, nSkipY;
	int nInspCount, nInspXCount;
	int nY1, nY2, nX1, nX2, nTmp, Maxj, Maxi, addValue;
	int nLL, nRR;
	long nProjMaxSum = 0, nProjMaxCount = 0, nProjMax = 0, nProjMaxPos[2];
	double dAveProj = 0, dAvgRatio = 1;
	long nAveValue = 0;
	int  nCountTT = 0, SValue[10], SValue1[10], Sx[10], Sy[10];
	int  nBASE_VALUE = 88;					 //기준밝기는 40
	int nOffsetX, nOffsetY, SValueSum, SValueSum1, nOverlap;
	int nInclude;
	int nMaxSValue, nMaxSPos;
	int nOverLapLimitY = 128, nOverLapLimitX = 128;
	int nRowNum, nColNum;
	int nLeftSum, nRightSum;
	int nOffset;

	nRowNum = bottom / nJumpY;  if (bottom % nJumpY)  nRowNum++;
	nColNum = right / 128;      if (right % 128)      nColNum++;
	if (nRowNum > 256 || nColNum > 64) goto done; //배열 잡은 게 [256][64] 임.

	if (nThUp < 200) nThUp = 200;				//200이하는 노이즈

	if (nErode < 2) nErode = 2;					 //1 이면  3   
	else if (nErode > 20)  nErode = 20;			 //10이면  21   

	if (nJumpY < 16)  nJumpY = 16;								//최소 JUMP 값은 16이다 		
	if ((bottom - top) <= nInspY || right - left <= 20) goto done;
	memset(_nMax, 0, nRowNum * 64 * sizeof(int));
	memset(_nMaxPos, 0, nRowNum * 64 * sizeof(int));
	memset(_pKProjFlatAvg, 0, pitch * sizeof(long));

	//Projection 데이타가 주어지면 그것을 이용하고 그렇지 않으면 전체 Projection을 구한다.---
	//0.87 msec
	if (pProj == NULL)
	{
		nSkipY = (bottom - top + 1) / nProjCount;
		if (nSkipY < 1) nSkipY = 1;
		nProjCount = 0;
		for (i = top; i < bottom; i += nSkipY) nProjCount++;
		if (nProjCount == 0) goto done;
		memset(_pKProj, 0, right * sizeof(long));

		for (i = top; i < bottom; i += nSkipY)
			for (j = left; j < right; j++)
				*(_pKProj + j) += *(fm + pitch * i + j);

		for (j = left; j < right; j++)       *(_pKProj + j) /= nProjCount;

		{
			memcpy(_pKProjLocal, _pKProj, right * sizeof(long));
			for (j = left + 2; j < right - 2; j++)
				_pKProj[j] = (_pKProjLocal[j - 2] + _pKProjLocal[j - 1] + _pKProjLocal[j] + _pKProjLocal[j + 1] + _pKProjLocal[j + 2]) / 5;
		}
	}
	else
	{
		for (j = left; j < right; j++)
			*(_pKProj + j) = *(pProj + j);
	}

	//전체 평균밝기 구해서 전체 평균밝기가 기준값보다 크면 그 만큼 결과를 작게한다.------------
	for (j = left; j < right; j++)
		nAveValue += *(_pKProj + j);


	nAveValue /= (right - left);
	if (nAveValue > nBASE_VALUE)
	{
		dAvgRatio = (double)nBASE_VALUE / (double)nAveValue;
		dAvgRatio = sqrt(dAvgRatio);
	}
	//-----------------------------------------------------------------------------------------

	nInspCount = nInspXCount = 0;
	for (i = top; i < bottom; i += nJumpY) nInspCount++;
	for (i = left; i < right; i += 128)    nInspXCount++;

	// 스크라치 아래 루틴
	// Parallel_for로 처리
	FindScratchSub(fm, left, right, bottom, pitch, _pKProj, nInspY, nJumpY, nUpperCut, _pKProjLocalArray);

	for (k = 0; k < nInspCount; k++)
	{
		nY1 = top + k * nJumpY;
		nY2 = nY1 + nInspY;
		if (nY2 > bottom) { nY2 = bottom; nY1 = nY2 - nInspY; }

		memset(_pKProjLocal, 0, right * sizeof(long));

		nLL = nRR = 0;

		memcpy(_pKProjLocal + left, _pKProjLocalArray + pitch * k + left, sizeof(int) * (right - left));

	//튀는 것만-----------------------------------------------------------------
			//ERODE------------------------------------------------------
		nOffset = 2;
		for (j = left + nOffset; j < right - nOffset; j++)
		{
			nTmp = *(_pKProjLocal + j - nOffset);
			for (m = j - nOffset + 1; m <= j + nOffset; m++)
				if (*(_pKProjLocal + m) < nTmp) nTmp = *(_pKProjLocal + m);

			_pKProjErode[j] = nTmp;
		}
		for (j = left; j < left + nOffset; j++)   _pKProjErode[j] = _pKProjLocal[j];
		for (j = right - nOffset; j < right; j++) _pKProjErode[j] = _pKProjLocal[j];

		//DILATE------------------------------------------------------
		for (j = left + nOffset; j < right - nOffset; j++)
		{
			nTmp = *(_pKProjErode + j - nOffset);
			for (m = j - nOffset + 1; m <= j + nOffset; m++)
				if (*(_pKProjErode + m) > nTmp) nTmp = *(_pKProjErode + m);

			_pKProjDilate[j] = nTmp;
		}
		for (j = left; j < left + nOffset; j++)   _pKProjDilate[j] = _pKProjErode[j];
		for (j = right - nOffset; j < right; j++) _pKProjDilate[j] = _pKProjErode[j];
		//------------------------------------------------------------------------------

		//주변밝기 대비 밝은 것 검출----------------------------------------------------
		nLeftSum = nRightSum = 0;
		for (j = left; j < left + nErode - 1; j++)
			nLeftSum += *(_pKProjLocal + j);
		for (j = left + nErode + 2; j <= left + nErode * 2; j++)
			nRightSum += *(_pKProjLocal + j);

		for (j = left + nErode; j < right - nErode; j++)
		{
			_pKProjErode[j] = (nLeftSum + nRightSum) / (nErode * 2 - 2);

			nLeftSum = nLeftSum - *(_pKProjLocal + j - nErode) + *(_pKProjLocal + j - 1);
			nRightSum = nRightSum - *(_pKProjLocal + j + 2) + *(_pKProjLocal + j + nErode + 1);
		}
		//-------------------------------------------------------------------------------


				//원래 Profile과 ERODE(5x1)-DILATE(5x1)한것과의 차이(Profile에서 없어진 부분만 구함)
		for (j = left; j < right; j++)
		{
			_pKProjFlat[j] = (_pKProjLocal[j] - _pKProjDilate[j]);    //원본 - (ERODE(5x1)-DILATE(5x1))
			_pKProjFlat1[j] = (_pKProjLocal[j] - _pKProjErode[j]);   //원본 - Average
			_pKProjFlatAvg[j] += _pKProjFlat[j];
		}


		//범위를 세로로는 nJumpY간격으로 가로로는 128간격으로 가장 높은 _pKProjFlat값들을 찾는다.
		for (m = 0; m < nInspXCount; m++)
		{
			if (m == 0) nX1 = left + nErode;
			else     nX1 = left + 128 * m;

			nX2 = nX1 + 128;
			if (nX2 > right - nErode)
				nX2 = right - nErode;

			for (j = nX1; j < nX2; j++)
			{
				if (_pKProjFlat[j] > _nMax[k][m])
				{
					if (j > nLL && j < nRR) //HOLE영역 무시 
					{

					}
					else
					{
						_nMax[k][m] = _pKProjFlat[j];
						_nMax1[k][m] = _pKProjFlat1[j];
						_nMaxPos[k][m] = j;
					}
				}
			}

			//Average구하기 위한값---------------------------
			nProjMaxSum += _nMax[k][m];
			nProjMaxCount++;


			//제일 높은값 찾기 (Display용)-------------------
			if (_nMax[k][m] > nProjMax)
			{
				nProjMax = _nMax[k][m];
				nProjMaxPos[0] = (nX1 + nX2) / 2;  //제일 쎈 스크라치 위치 
				nProjMaxPos[1] = (nY1 + nY2) / 2;
			}
			//----------------------------------------------
		}
	}

	//Scratch평균값 -------------------------------
	for (j = left; j < right; j++)
		_pKProjFlatAvg[j] /= nInspCount;
	//---------------------------------------------

	//전체 128영역에서 MAX의 평균값 ---------------------
	if (nProjMaxCount) dAveProj = nProjMaxSum / nProjMaxCount;

	*AverageScratch = (int)(dAveProj * dAvgRatio);
	*MaxScratch = (int)(nProjMax * dAvgRatio);

	//스크라치를 찾는다(최대 10개)-----------------------------------------
	memcpy(_nMaxBK, _nMax, nInspCount * 64 * sizeof(int));  //l_nMax를  l_nMaxBK에 백업받는다 (백업데이타를 이용해서 큰 순서대로 찾는다)
	for (k = 0; k < MAX_SCRATCH; k++)
	{
		nProjMax = 0;
		for (i = 0; i < nInspCount; i++)
			for (j = 0; j < nInspXCount; j++)
			{
				if (_nMaxBK[i][j] > nProjMax)
				{
					nProjMax = _nMaxBK[i][j];
					Maxi = i;
					Maxj = j;
				}
			}

		nInclude = 0;
		if (nProjMax > 100) //라인 스캔을 48x3영역에서 더함 (Area는 32x5(중복포함))
		{
			SValueSum = nProjMax;
			SValueSum1 = _nMax1[Maxi][Maxj];

			//----------------------------------------------------------------------------------------
					//긴 Scratch에 대한 가중치 Local Max가 발생한 x좌표의 +2,-2위치에서 
			if (Maxi > 1)
			{
				if (abs(_nMaxPos[Maxi][Maxj] - _nMaxPos[Maxi - 2][Maxj]) <= 2)
				{
					addValue = _nMax[Maxi - 2][Maxj] - _pKProjFlatAvg[_nMaxPos[Maxi][Maxj]];
					if (addValue > 75) SValueSum += addValue;

					addValue = _nMax1[Maxi - 2][Maxj] - _pKProjFlatAvg[_nMaxPos[Maxi][Maxj]];
					if (addValue > 75) SValueSum1 += addValue;
				}
				nInclude++;
			}
			if (Maxi < nInspCount - 2)
			{
				if (abs(_nMaxPos[Maxi][Maxj] - _nMaxPos[Maxi + 2][Maxj]) <= 2)
				{
					addValue = _nMax[Maxi + 2][Maxj] - _pKProjFlatAvg[_nMaxPos[Maxi][Maxj]];
					if (addValue > 75) SValueSum += addValue;

					addValue = _nMax1[Maxi + 2][Maxj] - _pKProjFlatAvg[_nMaxPos[Maxi][Maxj]];
					if (addValue > 75) SValueSum1 += addValue;
				}
				nInclude++;
			}
			if (Maxi > 0 && nInclude < 2)
			{
				if (abs(_nMaxPos[Maxi][Maxj] - _nMaxPos[Maxi - 1][Maxj]) <= 2)
				{
					addValue = _nMax[Maxi - 1][Maxj] - _pKProjFlatAvg[_nMaxPos[Maxi][Maxj]];
					if (addValue > 75) SValueSum += addValue;

					addValue = _nMax1[Maxi - 1][Maxj] - _pKProjFlatAvg[_nMaxPos[Maxi][Maxj]];
					if (addValue > 75) SValueSum1 += addValue;
				}
				nInclude++;
			}

			if (Maxi < nInspCount - 1 && nInclude < 2)
			{
				if (abs(_nMaxPos[Maxi][Maxj] - _nMaxPos[Maxi + 1][Maxj]) <= 2)
				{
					addValue = _nMax[Maxi + 1][Maxj] - _pKProjFlatAvg[_nMaxPos[Maxi][Maxj]];
					if (addValue > 75) SValueSum += addValue;

					addValue = _nMax1[Maxi + 1][Maxj] - _pKProjFlatAvg[_nMaxPos[Maxi][Maxj]];
					if (addValue > 75) SValueSum1 += addValue;
				}
			}
			//----------------------------------------------------------------------------------------
			nOverlap = 0;


			for (i = 0; i < nScratchCount; i++)
			{
				nOffsetY = top + nInspY / 2 + nJumpY * Maxi - Sy[i];
				if (nOffsetY < 0) nOffsetY *= -1;
				nOffsetX = _nMaxPos[Maxi][Maxj] - Sx[i];
				if (nOffsetX < 0) nOffsetX *= -1;
				if (nOffsetY < nOverLapLimitY && nOffsetX < nOverLapLimitX) //같은 위치이면OVERLAP
				{
					nOverlap = 1;
					if (SValueSum > SValue[i]) //현재 Value가 크면 대체한다.
					{
						SValue[i] = SValueSum;
						SValue1[i] = SValueSum1;
						Sx[i] = _nMaxPos[Maxi][Maxj];
						Sy[i] = top + nInspY / 2 + nJumpY * Maxi;
					}
					break;
				}
			}

			if (nOverlap == 0)
			{
				SValue[nScratchCount] = SValueSum;
				SValue1[nScratchCount] = SValueSum1;
				Sx[nScratchCount] = _nMaxPos[Maxi][Maxj];
				Sy[nScratchCount] = top + nInspY / 2 + nJumpY * Maxi;
				nScratchCount++;
			}
			_nMaxBK[Maxi][Maxj] = 0; //1
			if (Maxi > 0)			  _nMaxBK[Maxi - 1][Maxj] = 0;
			if (Maxi < nInspCount - 1) _nMaxBK[Maxi + 1][Maxj] = 0;
		}
		else
		{
			break;
		}
	}

	//제일큰 스크라치 찾는다 (Display용)--
	nMaxSPos = 0;
	for (i = 0; i < nScratchCount; i++)
	{
		SValue[i] = (int)((double)SValue[i] * dAvgRatio);
		if (SValue[i] > *MaxScratch)
		{
			*MaxScratch = SValue[i];
			nMaxSPos = i;
		}
	}
	if (SValue1[nMaxSPos] < *MaxScratch)
		*MaxScratch = SValue1[nMaxSPos];  //두가지 방법중 작은 것을 사용한다.
	//------------------------------------

	//제일 큰 순서대로 소팅해서 RETURN-------------------------------------------------
	nCountTT = nScratchCount;
	nScratchCount = 0;
	for (i = 0; i < nCountTT; i++)
	{
		//제일 큰 것 찾는다-------------
		nMaxSValue = 0;
		for (j = 0; j < nCountTT; j++)
		{
			if (SValue[j] > nMaxSValue)
			{
				nMaxSValue = SValue[j];
				nMaxSPos = j;
			}
		}
		//------------------------------

		if (SValue[nMaxSPos] >= nThUp && SValue1[nMaxSPos] >= nThUp)
		{
			//겹치는 것이 있는지 체크한다.----------------------------
			nOverlap = 0;
			for (k = 0; k < nScratchCount; k++)
			{
				nOffsetY = posy[k] - Sy[nMaxSPos];
				if (nOffsetY < 0) nOffsetY *= -1;
				nOffsetX = posx[k] - Sx[nMaxSPos];
				if (nOffsetX < 0) nOffsetX *= -1;
				if (nOffsetY < nOverLapLimitY && nOffsetX < nOverLapLimitX)
					nOverlap = 1;
			}
			//--------------------------------------------------------

			//겹치는 것이 었으면 데이타에 넣는다. (최대 3개)
			if (nOverlap == 0 && nScratchCount < 3)
			{
				if (SValue[nMaxSPos] < SValue1[nMaxSPos]) value[nScratchCount] = SValue[nMaxSPos];
				else                                   value[nScratchCount] = SValue1[nMaxSPos];

				posx[nScratchCount] = Sx[nMaxSPos];
				posy[nScratchCount] = Sy[nMaxSPos];

				nScratchCount++;
				SValue[nMaxSPos] = 0;
			}
		}
	}
	//------------------------------------------------------------

done:

	return nScratchCount;
}

void Inspect::FindScratchSub(LPBYTE fm, int nX1, int nX2, int nHeight, int pitch, long* pKProj, int  nInspY, int nJumpY, int  nUpperCut, int* pKProjLocalArray)
{
	int nTotalCount;
	nTotalCount = nHeight / nJumpY;

	Concurrency::parallel_for(size_t(0), size_t(nHeight), size_t(nJumpY), [&](size_t nY1)
	{
		int i, j;
		int nY2, nAvg, nTmp;
		int nUpperCutCount, nTmpSum, nMax;

		nY2 = (int)(nY1 + nInspY);
		if (nY2 > nHeight) { nY2 = nHeight; nY1 = nY2 - nInspY; }

		for (j = nX1; j < nX2; j++)
		{
			nAvg = *(pKProj + j);
			nUpperCutCount = 0;
			nTmpSum = 0;
			nMax = 0;

			for (i = nY1; i < nY2; i++)
			{
				nTmp = *(fm + pitch * i + j) - nAvg;
				if (nTmp > nMax) nMax = nTmp;

				if (nTmp > nUpperCut)
				{
					if (nUpperCutCount < 4) nTmp = nAvg;          //Uppercut초과하는 4개까지 없앰(평균값넣음. 휘점이 스크라치로 분류되는 것 막음)
					else                 nTmp = nAvg + nUpperCut;
					nUpperCutCount++;
				}
				else
					nTmp += nAvg;

				nTmpSum += nTmp;
			}

			//일단 최대 4개의 튄값은 뺀다.(nUpperCut을 넘어간건 이미 뻇으므로 할 필요없다------------------
			if (nUpperCutCount < 4)
			{
				if (nMax > nUpperCut) nMax = nUpperCut;
				nTmpSum -= (nMax * (4 - nUpperCutCount));
			}
			//---------------------------------------------------------------------------------------------

			*(pKProjLocalArray + (nY1 / nJumpY) * pitch + j) = nTmpSum * 100 / nInspY;
		}
	});
}