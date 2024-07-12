#pragma once
#include "../stdafx.h"
#include "atltypes.h"
#include "../Param/Param.h"
#include "../Param/TempParam.h"
#include "../PreProcess/PreProc.h"
#include "../PreProcess/PyramidImage.h"
#include "../Param/SystemParam.h"
#include "../Fchain.h"
#include "../Defines.h"
#include "../Defect/DefectData.h"
#include "../Param/MarkParam.h"
#include "../Param/DefectType.h"

class Inspect
{
private:

	SystemParam*			_pSystem;		// 시스템 파라미터 (주소만 얻어옴)
	Param*					_pParam;		// 검사 파라미터 (주소만 얻어옴)
	TempParam::TEMP_PARAM*	_pTmpData;		// 검사 임시 파라미터 (주소만 얻어옴)
	MARK_OPTIC*				_pMark;			// 결점 마킹 처리 데이터

	FlatImage*				_pFlat;			// 이미지 평활화 클래스
	Profile*				_pProfile;		// 이미지 프로파일 클래스
	FindEdge*				_pEdgeFinder;	// 에지 검출 클래스	
	PyramidImage*			_pPyramid;		// 피라미드 영상 생성
	PyramidImage*			_pPyramidSc;	// 피라미드 영상 생성
	CChain*					_pChain;		// Blob용 Chain Code	

	// 이미지 데이터
	int _width;
	int _height;

	unsigned char*			_SrcImg;	// 원본 영상 (데이터 참조)
	unsigned char*			_pFlatImg;	// 평활화 영상 (여기에서 생성)

	InspParam::CANDI_POINTS	_CandiW;		// 백점 후보
	InspParam::CANDI_POINTS	_CandiB;		// 흑점 후보
	InspParam::CANDI_POINTS	_CandiArea;		// 찍힘 후보

	// 조각 영상 처리용 버퍼
	unsigned char			_fmBin[BAD_IMG_HEIGHT * BAD_IMG_WIDTH];

	// 전체 영상 임시 버퍼
	LPBYTE					_pFmTemp;
	
	/////////////////////////////////////////////////////////////////////
	// 스크라치 검사용 버퍼
	int _nMaxBK[256][64], _nMaxPos[256][64];		//세로 최대 128, 가로는 128씩 체크 
	int _nMax[256][64], _nMax1[256][64];
	long *_pKProjFlatAvg;
	long *_pKProj, *_pKProjLocal;
	int *_pKProjLocalArray;
	long *_pKProjErode, * _pKProjDilate;
	long  *_pKProjFlat, *_pKProjFlat1, *_pKProjFlatMax;
	/////////////////////////////////////////////////////////////////////

	// 검사 결과 
	DEFECTDATA				_DefectData;

public:
	Inspect();
	virtual ~Inspect();

	void Init();

	void SetParam(Param* pParam);
	void SetTempParam(TempParam::TEMP_PARAM* pTmpParam);

	virtual void Create(int width, int height);
	virtual void Release();

	virtual void Run();

	/// <summary>
	/// 검사 영상 메모리 참조 할당
	/// </summary>
	/// <param name="pSrc"></param>
	void SetSrcImage(unsigned char* pSrc) { _SrcImg = pSrc; }

	/// <summary>
	/// 평활화 영상 참주 주소 전달
	/// </summary>
	/// <returns></returns>
	unsigned char* GetFlatImage() { return _pFlatImg; }

	/// <summary>
	/// 피라미드 영상 생성
	/// </summary>
	void MakePyramid();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Inspect 위치

	/// <summary>
	/// 기본 검사
	/// </summary>
	void SpotInspect();


	/// <summary>
	/// 스크래치 검사
	/// </summary>
	void SetScInspBuf(int width, int height);
	void ScInspect();
	int FindScratch(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nType, int nUpperCut, int nLowerCut, int nInspY, int nJumpY, int nErode,
		int nThUp, int nThDn, int nMulti, int* pProj, int* posx, int* posy, int* value, int* AverageScratch, int* MaxScratch);
	void FindScratchSub(LPBYTE fm, int nX1, int nX2, int nHeight, int pitch, long* pKProj, int  nInspY, int nJumpY, int  nUpperCut, int* pKProjLocalArray);
	// Inspect 위치
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InspectSub 위치

	/// <summary>
	/// 후보점 찾기
	/// </summary>
	void FindCandidate(int nStartX, int nWidth, int nGap);

	/// <summary>
	/// 후보점 찾기 병렬 처리
	/// </summary>
	void FindCandidateParallel(int nStartX, int nWidth, int nGap);

	/// <summary>
	/// 
	/// </summary>
	int FindCandiSorting(int nStartX, int nWidth, int nGap);

	/// <summary>
	/// 경계투과(반사) 면 이미지가 울렁거려 단순울렁인건지 Defect인지 한번더 판단한다.
	/// </summary>
	int CheckBoundaryOpticOverKill(unsigned char* fm, int nX, int nY, int pitch, int nGrabHeight, int Value);

	/// <summary>
	/// 단순한 Gradient인지 검증
	/// 실제영역 64x64에서 밝기 변화가 일정값 만큼 없으면 과검(단순 Gradient변화) 으로 간주함
	/// (nCenterX, nCenterY) Defect 위치
	/// dLimitvalue : 8x8이웃하는 Pixel간의 최대 밝기 차
	/// X1, X2 : 영상 사용가능 최대 범위(X)
	/// nBottom: 영상 사용가능 최대 범위(Y)
	/// </summary>
	int CheckSimpleGradient(int nCenterX, int nCenterY, int X1, int X2, int nBottom, double dLimitvalue);


	void MakeDefectRect(int nDefect, int nX, int nY, int nLeft, int nRight);

	/// <summary>
	/// 
	/// </summary>
	void MakeDefectRect_BigDefect(int nX, int nY, int nLeft, int nRight, int* left, int* top);
	void MakeDefectRect_BigDefect1(int nDefect, int nX, int nY, int nLeft, int nRight);

	int		BinaryDilate(unsigned char* fmS, unsigned char* fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
	int		BinaryErode(unsigned char* fmS, unsigned char* fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
	int		GetAveValueByHistogram(unsigned char* fm, int left, int top, int width, int height, int pitch);
	void	GetSquireMinMax(unsigned char l1, unsigned char l2, unsigned char l3, unsigned char l4, int* pMax, int* pMin);
	double	GetNittoSize(int nBlobNo, CRect& rt);
	double	GetNeSize(int nBlobNo, CRect& rt);

	void GetSizeNValue(int nType, unsigned char* fm, int nDefectY, int left, int top, int width, int height, int pitch, int nAveValue, int nThUp, int nThDn, int nAttachPixel, int nSizeMethod,
		double dScaleX, double dScaleY, int* pAvg, int* pValueUp, int* pValueDn, double* pSize, double* pImulKipoSize, int* pCx, int* pCy, RECT* realRect);

	int SelectDefect(int nLevel1, int nClass1, int nLevel2, int nClass2);

	//Level값 얻는 함수
	int GetLevel(double dValue, double* pValue);
	int GetLevel(int dValue, int* pValue);
	int GetLevel(double dValue, double dSize, int nAvg, int* pValue, double* pSize);
	int GetLevel(double dValueUp, double dValueDn, double dSize, int* pValueUp, int* pValueDn, double* pSize);
	int GetLevel(double dValue, double dSize, double* pValue, double* pSize);

	int CopyNGImage(bool bFlat);
	int CopyNGImageTemp(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch);
	int CopyNGImageFMArea(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGray);
	int CopyNGImageFM(LPBYTE fm, int pitch);

	int CheckDefectOverlap(double instRngX = 0.0, double instRngY = 0.0);
	void CheckPriorLevel(int nIsPrior);


	int IsOverKillForScratch(LPBYTE fm, int nX, int nY, int pitch, int nHeight, int nValue);
	int CheckRealScratch(LPBYTE fm, int nX, int nY, int pitch, int nMaxWidth, int nMaxHeight, int nMinLength);
	void BlobElongation(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinLength);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

