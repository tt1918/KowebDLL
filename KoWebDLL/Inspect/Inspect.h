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

	SystemParam*			_pSystem;		// �ý��� �Ķ���� (�ּҸ� ����)
	Param*					_pParam;		// �˻� �Ķ���� (�ּҸ� ����)
	TempParam::TEMP_PARAM*	_pTmpData;		// �˻� �ӽ� �Ķ���� (�ּҸ� ����)
	MARK_OPTIC*				_pMark;			// ���� ��ŷ ó�� ������

	FlatImage*				_pFlat;			// �̹��� ��Ȱȭ Ŭ����
	Profile*				_pProfile;		// �̹��� �������� Ŭ����
	FindEdge*				_pEdgeFinder;	// ���� ���� Ŭ����	
	PyramidImage*			_pPyramid;		// �Ƕ�̵� ���� ����
	PyramidImage*			_pPyramidSc;	// �Ƕ�̵� ���� ����
	CChain*					_pChain;		// Blob�� Chain Code	

	// �̹��� ������
	int _width;
	int _height;

	unsigned char*			_SrcImg;	// ���� ���� (������ ����)
	unsigned char*			_pFlatImg;	// ��Ȱȭ ���� (���⿡�� ����)

	InspParam::CANDI_POINTS	_CandiW;		// ���� �ĺ�
	InspParam::CANDI_POINTS	_CandiB;		// ���� �ĺ�
	InspParam::CANDI_POINTS	_CandiArea;		// ���� �ĺ�

	// ���� ���� ó���� ����
	unsigned char			_fmBin[BAD_IMG_HEIGHT * BAD_IMG_WIDTH];

	// ��ü ���� �ӽ� ����
	LPBYTE					_pFmTemp;
	
	/////////////////////////////////////////////////////////////////////
	// ��ũ��ġ �˻�� ����
	int _nMaxBK[256][64], _nMaxPos[256][64];		//���� �ִ� 128, ���δ� 128�� üũ 
	int _nMax[256][64], _nMax1[256][64];
	long *_pKProjFlatAvg;
	long *_pKProj, *_pKProjLocal;
	int *_pKProjLocalArray;
	long *_pKProjErode, * _pKProjDilate;
	long  *_pKProjFlat, *_pKProjFlat1, *_pKProjFlatMax;
	/////////////////////////////////////////////////////////////////////

	// �˻� ��� 
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
	/// �˻� ���� �޸� ���� �Ҵ�
	/// </summary>
	/// <param name="pSrc"></param>
	void SetSrcImage(unsigned char* pSrc) { _SrcImg = pSrc; }

	/// <summary>
	/// ��Ȱȭ ���� ���� �ּ� ����
	/// </summary>
	/// <returns></returns>
	unsigned char* GetFlatImage() { return _pFlatImg; }

	/// <summary>
	/// �Ƕ�̵� ���� ����
	/// </summary>
	void MakePyramid();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Inspect ��ġ

	/// <summary>
	/// �⺻ �˻�
	/// </summary>
	void SpotInspect();


	/// <summary>
	/// ��ũ��ġ �˻�
	/// </summary>
	void SetScInspBuf(int width, int height);
	void ScInspect();
	int FindScratch(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nType, int nUpperCut, int nLowerCut, int nInspY, int nJumpY, int nErode,
		int nThUp, int nThDn, int nMulti, int* pProj, int* posx, int* posy, int* value, int* AverageScratch, int* MaxScratch);
	void FindScratchSub(LPBYTE fm, int nX1, int nX2, int nHeight, int pitch, long* pKProj, int  nInspY, int nJumpY, int  nUpperCut, int* pKProjLocalArray);
	// Inspect ��ġ
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InspectSub ��ġ

	/// <summary>
	/// �ĺ��� ã��
	/// </summary>
	void FindCandidate(int nStartX, int nWidth, int nGap);

	/// <summary>
	/// �ĺ��� ã�� ���� ó��
	/// </summary>
	void FindCandidateParallel(int nStartX, int nWidth, int nGap);

	/// <summary>
	/// 
	/// </summary>
	int FindCandiSorting(int nStartX, int nWidth, int nGap);

	/// <summary>
	/// �������(�ݻ�) �� �̹����� �ﷷ�ŷ� �ܼ��ﷷ�ΰ��� Defect���� �ѹ��� �Ǵ��Ѵ�.
	/// </summary>
	int CheckBoundaryOpticOverKill(unsigned char* fm, int nX, int nY, int pitch, int nGrabHeight, int Value);

	/// <summary>
	/// �ܼ��� Gradient���� ����
	/// �������� 64x64���� ��� ��ȭ�� ������ ��ŭ ������ ����(�ܼ� Gradient��ȭ) ���� ������
	/// (nCenterX, nCenterY) Defect ��ġ
	/// dLimitvalue : 8x8�̿��ϴ� Pixel���� �ִ� ��� ��
	/// X1, X2 : ���� ��밡�� �ִ� ����(X)
	/// nBottom: ���� ��밡�� �ִ� ����(Y)
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

	//Level�� ��� �Լ�
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

