#include "Inspect.h"
#include <ppl.h>
#include <cmath>


void Inspect::FindCandidate(int nStartX, int nWidth, int nGap)
{
	int i, j, n, m;

	if (nWidth > 4096)
	{
		if (nGap < 256) nGap = 256;
	}

	int nN = nStartX / nGap;
	int nEndX = nWidth;
	int nNW;
	int nM, nDiff;
	int nNPitch = _pPyramid->GetImageWidth(0) / nGap;
	int nCandiCount = 0;
	int nX1, nX2, nY1, nY2, pitch;
	int nLastGap = 1;
	unsigned char* fmT, * fmT_;
	int nID;
	int nFlatBase = _pSystem->FlatBright;
	int nTmp, nTmp_, nSum, nFlag, nMax, nMaxDiff, nMaxPosX, nMaxPosY, nMin, nMinPosX, nMinPosY;
	int nLimitX1 = _pTmpData->InspArea.X1 / 2;
	int nLimitX2 = _pTmpData->InspArea.X2 / 2;
	int nLimitLastX1;
	int nLimitLastX2;
	int nTinyMaxB;

	int nPyWidth[COMP_LEVEL];
	int nPyHeight[COMP_LEVEL];
	unsigned char* pPyBuf[COMP_LEVEL];
	unsigned char* pTarBuf;

	nPyWidth[0] = _pPyramid->GetImageWidth(0);
	nPyHeight[0] = _pPyramid->GetImageHeight(0);
	pPyBuf[0] = _pFlatImg;//_pPyramid->GetImagePt(0);

	for (i = 1; i < COMP_LEVEL; i++)
	{
		nLastGap = nLastGap * 2;
		nPyWidth[i] = _pPyramid->GetImageWidth(i);
		nPyHeight[i] = _pPyramid->GetImageHeight(i);
		pPyBuf[i] = _pPyramid->GetImagePt(i);
	}

	nLimitLastX1 = _pTmpData->InspArea.X1 / nLastGap;
	nLimitLastX2 = _pTmpData->InspArea.X2 / nLastGap;

	if (nEndX > nPyWidth[0])	nEndX = nPyWidth[0];

	//(nStartX+nWidth)가 nGap의 배수가 아니면 아래와 같이처리----------
	nNW = (nStartX + nWidth) / nGap;
	if ((nStartX + nWidth) - nNW * nGap > 0)      nNW++;

	//l_nPyramidHeight[0]가 nGap의 배수가 아니면 아래와 같이처리-------
	nM = nPyWidth[0] / nGap;
	if (nPyWidth[0] - nM * nGap > 0)    nM++;

	//전체영역이 nGap의 배수가 아니면 
	nNPitch = nPyWidth[0] / nGap;
	if (nPyWidth[0] > nNPitch * nGap)  nNPitch++;
	//-----------------------------------------------------------------

	_CandiW.Reset();
	_CandiB.Reset();
	_CandiArea.Reset();

	for (n = 0; n < nM; n++)
	{
		for (m = nN; m < nNW; m++)
		{
			nID = n * nNPitch + m;
			nID = n * nNPitch + m;

			//(1/2)영역에서 후보 구함 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			nX1 = m * nGap / 2;			 if (nX1 < nStartX / 2)   nX1 = nStartX / 2;
			nX2 = (m + 1) * nGap / 2;		 if (nX2 > nEndX / 2)     nX2 = nEndX / 2;
			nY1 = n * nGap / 2;
			nY2 = (n + 1) * nGap / 2;		 if (nY2 > nPyHeight[1]) nY2 = nPyHeight[1];
			pitch = nPyWidth[1];

			//미검영역 적용-------------------
			if (nX1 < nLimitX1) nX1 = nLimitX1;
			if (nX2 > nLimitX2) nX2 = nLimitX2;
			if (nX2 - nX1 < 10 || nY2 - nY1 == 0) continue;  // divide zero 예외 처리
			//--------------------------------

			nSum = 0;
			nMax = -1;
			nMin = 256;
			nTinyMaxB = 0;
			pTarBuf = pPyBuf[1];
			for (i = nY1; i < nY2; i++)
			{
				fmT = pTarBuf + i * pitch + nX1;
				for (j = nX1; j < nX2; j++)
				{
					nTmp = *(fmT++);
					nSum += nTmp;
					if (nTmp > nMax)
					{
						nMax = nTmp;
						nMaxPosX = j;
						nMaxPosY = i;
					}
					if (nTmp < nMin)
					{
						nMin = nTmp;
						nMinPosX = j;
						nMinPosY = i;
					}

				}
			}

			_CandiW.SetData(nID, nMax, nMaxPosX * 2 + 1, nMaxPosY * 2 + 1, nSum / ((nY2 - nY1) * (nX2 - nX1)));
			_CandiB.SetData(nID, nMin, nMinPosX * 2 + 1, nMinPosY * 2 + 1, nSum / ((nY2 - nY1) * (nX2 - nX1)));

			//(1/nLastGap)영역에서 후보 구함 >>>>>>>>>>>>>>>>>>>>
			nX1 = m * nGap / nLastGap;				if (nX1 < nStartX / nLastGap)  nX1 = nStartX / nLastGap;
			nX2 = (m + 1) * nGap / nLastGap;			if (nX2 > nEndX / nLastGap)	  nX2 = nEndX / nLastGap;
			nY1 = n * nGap / nLastGap;
			nY2 = (n + 1) * nGap / nLastGap;			if (nY2 > nPyHeight[COMP_LEVEL - 1]) nY2 = nPyHeight[COMP_LEVEL - 1];
			pitch = nPyWidth[COMP_LEVEL - 1];

			//미검영역 적용-----------------------
			if (nX1 < nLimitLastX1) nX1 = nLimitLastX1;
			if (nX2 > nLimitLastX2) nX2 = nLimitLastX2;
			if (nX2 - nX1 < 1) continue;
			//------------------------------------

			nSum = 0;
			nMaxDiff = -1;
			nMax = 0;
			nMin = 256;
			pTarBuf = pPyBuf[COMP_LEVEL - 1];

			for (i = nY1; i < nY2 - 1; i++)
			{
				fmT = pTarBuf + i * pitch + nX1;
				fmT_ = pTarBuf + (i + 1) * pitch + nX1;
				for (j = nX1; j < nX2; j++)
				{
					nTmp = *(fmT++);
					nTmp_ = *(fmT_++);

					nDiff = nTmp - nTmp_; if (nDiff < 0) nDiff *= -1;

					if (nDiff > nMaxDiff)
					{
						nMaxDiff = nDiff;
						nMaxPosX = j;
						nMaxPosY = i;
						if (nTmp > nTmp_)
						{
							if (nTmp - nFlatBase >= nFlatBase - nTmp_) nFlag = 1;
							else                                nFlag = -1;
						}
						else
						{
							if (nTmp_ - nFlatBase >= nFlatBase - nTmp) nFlag = 1;
							else                                nFlag = -1;
						}
					}
				}
			}
			_CandiArea.SetData(nID, nMaxDiff, nMaxPosX * nLastGap + nLastGap / 2, nMaxPosY * nLastGap + nLastGap, nFlag);
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		}
	}
}

void Inspect::FindCandidateParallel(int nStartX, int nWidth, int nGap)
{
	if (nWidth > 4096)
	{
		if (nGap < 256) nGap = 256;
	}

	int nN = nStartX / nGap;
	int nEndX = nWidth;
	int nNW;
	int nM;
	int nNPitch = _pPyramid->GetImageWidth(0) / nGap;
	int nCandiCount = 0;
	int nLastGap = 1;

	int nFlatBase = _pSystem->FlatBright;

	int nLimitLastX1, nLimitLastX2;

	////////////////////////////////////////////////////////
	int nPyWidth[COMP_LEVEL];
	int nPyHeight[COMP_LEVEL];
	unsigned char* pPyBuf[COMP_LEVEL];

	nPyWidth[0] = _pPyramid->GetImageWidth(0);
	nPyHeight[0] = _pPyramid->GetImageHeight(0);
	pPyBuf[0] = _pFlatImg;//_pPyramid->GetImagePt(0);
	//////////////////////////////////////////////////////////

	for (int i = 1; i < COMP_LEVEL; i++)
	{
		nLastGap = nLastGap * 2;
		nPyWidth[i] = _pPyramid->GetImageWidth(i);
		nPyHeight[i] = _pPyramid->GetImageHeight(i);
		pPyBuf[i] = _pPyramid->GetImagePt(i);
	}

	nLimitLastX1 = _pTmpData->InspArea.X1 / nLastGap;
	nLimitLastX2 = _pTmpData->InspArea.X2 / nLastGap;

	if (nEndX > nPyWidth[0])	nEndX = nPyWidth[0];

	//(nStartX+nWidth)가 nGap의 배수가 아니면 아래와 같이처리----------
	nNW = (nStartX + nWidth) / nGap;
	if ((nStartX + nWidth) - nNW * nGap > 0)      nNW++;

	//l_nPyramidHeight[0]가 nGap의 배수가 아니면 아래와 같이처리-------
	nM = nPyHeight[0] / nGap;
	if (nPyHeight[0] - nM * nGap > 0)    nM++;

	//전체영역이 nGap의 배수가 아니면 
	nNPitch = nPyWidth[0] / nGap; 
	if (nPyWidth[0] > nNPitch * nGap)  nNPitch++;
	//-----------------------------------------------------------------

	// 여기서 배열 초기화를 처리한다. 
	_CandiW.Init(nNW * nM);
	_CandiB.Init(nNW * nM);
	_CandiArea.Init(nNW * nM);

	_CandiW.Reset();
	_CandiB.Reset();
	_CandiArea.Reset();

	_CandiW.Count = nNW * nM;
	_CandiB.Count = nNW * nM;
	_CandiArea.Count = nNW * nM;

	Concurrency::parallel_for(0, nM, [&](int n) {
//	for (int n = 0; n < nM; n++)
//	{
		int nLimitX1 = _pTmpData->InspArea.X1 / 2;
		int nLimitX2 = _pTmpData->InspArea.X2 / 2;

		int i, j, m;
		int nDiff;

		unsigned char* fmT, * fmT_;
		int nID;
		int nX1, nX2, nY1, nY2, pitch;
		int nTmp, nTmp_, nSum, nFlag, nMax, nMaxDiff, nMaxPosX, nMaxPosY, nMin, nMinPosX, nMinPosY;
		unsigned char* pTarBuf;

		for (m = nN; m < nNW; m++)
		{
			nID = n * nNPitch + m;
			nID = n * nNPitch + m;

			//(1/2)영역에서 후보 구함 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			nX1 = m * nGap / 2;			 if (nX1 < nStartX / 2)   nX1 = nStartX / 2;
			nX2 = (m + 1) * nGap / 2;		 if (nX2 > nEndX / 2)     nX2 = nEndX / 2;
			nY1 = n * nGap / 2;
			nY2 = (n + 1) * nGap / 2;		 if (nY2 > nPyHeight[1]) nY2 = nPyHeight[1];
			pitch = nPyWidth[1];

			//미검영역 적용-------------------
			if (nX1 < nLimitX1) nX1 = nLimitX1;
			if (nX2 > nLimitX2) nX2 = nLimitX2;
			if (nX2 - nX1 < 10 || nY2 - nY1 == 0) continue;  // divide zero 예외 처리
			//--------------------------------

			nSum = 0;
			nMax = -1;
			nMin = 256;

			pTarBuf = pPyBuf[1];
			for (i = nY1; i < nY2; i++)
			{
				fmT = pTarBuf + i * pitch + nX1;
				for (j = nX1; j < nX2; j++)
				{
					nTmp = *(fmT++);
					nSum += nTmp;
					if (nTmp > nMax)
					{
						nMax = nTmp;
						nMaxPosX = j;
						nMaxPosY = i;
					}
					if (nTmp < nMin)
					{
						nMin = nTmp;
						nMinPosX = j;
						nMinPosY = i;
					}

				}
			}

			_CandiW.SetData(nID, nMax, nMaxPosX * 2 + 1, nMaxPosY * 2 + 1, nSum / ((nY2 - nY1) * (nX2 - nX1)));
			_CandiB.SetData(nID, nMin, nMinPosX * 2 + 1, nMinPosY * 2 + 1, nSum / ((nY2 - nY1) * (nX2 - nX1)));

			//(1/nLastGap)영역에서 후보 구함 >>>>>>>>>>>>>>>>>>>>
			nX1 = m * nGap / nLastGap;				if (nX1 < nStartX / nLastGap)  nX1 = nStartX / nLastGap;
			nX2 = (m + 1) * nGap / nLastGap;			if (nX2 > nEndX / nLastGap)	  nX2 = nEndX / nLastGap;
			nY1 = n * nGap / nLastGap;
			nY2 = (n + 1) * nGap / nLastGap;			if (nY2 > nPyHeight[COMP_LEVEL - 1]) nY2 = nPyHeight[COMP_LEVEL - 1];
			pitch = nPyWidth[COMP_LEVEL - 1];

			//미검영역 적용-----------------------
			if (nX1 < nLimitLastX1) nX1 = nLimitLastX1;
			if (nX2 > nLimitLastX2) nX2 = nLimitLastX2;
			if (nX2 - nX1 < 1) continue;
			//------------------------------------

			nSum = 0;
			nMaxDiff = -1;
			nMax = 0;
			nMin = 256;
			pTarBuf = pPyBuf[COMP_LEVEL - 1];

			for (i = nY1; i < nY2 - 1; i++)
			{
				fmT = pTarBuf + i * pitch + nX1;
				fmT_ = pTarBuf + (i + 1) * pitch + nX1;
				for (j = nX1; j < nX2; j++)
				{
					nTmp = *(fmT++);
					nTmp_ = *(fmT_++);

					nDiff = nTmp - nTmp_; if (nDiff < 0) nDiff *= -1;

					if (nDiff > nMaxDiff)
					{
						nMaxDiff = nDiff;
						nMaxPosX = j;
						nMaxPosY = i;
						if (nTmp > nTmp_)
						{
							if (nTmp - nFlatBase >= nFlatBase - nTmp_) nFlag = 1;
							else                                nFlag = -1;
						}
						else
						{
							if (nTmp_ - nFlatBase >= nFlatBase - nTmp) nFlag = 1;
							else                                nFlag = -1;
						}
					}
				}
			}
			_CandiArea.SetData(nID, nMaxDiff, nMaxPosX * nLastGap + nLastGap / 2, nMaxPosY * nLastGap + nLastGap, nFlag);
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		}
	});
//	}
}

int Inspect::FindCandiSorting(int nStartX, int nWidth, int nGap)
{
	int i, j, k, m;
	int nCol, nRow, nNPitch;
	int nTmp, nMax, nPos, nOverlap;
	int nLimitY = 128;
	int nPosX1, nPosX2;
	int nMaxLoop;
	int MARKING_X_OFFSET = _pSystem->OverlapX;
	int MARKING_Y_OFFSET = _pSystem->OverlapY;
	if (MARKING_X_OFFSET <= 0) MARKING_X_OFFSET = 1;
	if (MARKING_Y_OFFSET <= 0) MARKING_Y_OFFSET = 1;


	InspParam::CANDI_POINTS candiTemp;
	InspParam::CANDI_POINTS* pCandi;

	if (nWidth > 4096)
	{
		if (nGap < 256) nGap = 256;
	}

	nCol = (nWidth - nStartX) / nGap;				if ((nWidth - nStartX) - nCol * nGap)		 nCol++;
	nRow = _pPyramid->GetImageHeight(0) / nGap;		if (_pPyramid->GetImageHeight(0) - nRow * nGap)	 nRow++;
	nNPitch = _pPyramid->GetImageWidth(0) / nGap;	if (_pPyramid->GetImageWidth(0) > nNPitch * nGap)  nNPitch++;

	/*_CandiW.Count = nCol * nRow;
	_CandiB.Count = nCol * nRow;
	_CandiArea.Count = nCol * nRow;*/

	if (_pParam->Cam.ScaleY > 0) nLimitY = (int)(MARKING_Y_OFFSET / _pParam->Cam.ScaleY);

	// 검출 후보 영역 검색
	// 검출 후보가 최대 후보 영역 갯수 보다 많으면 후보 영역 갯수 제한
	nMaxLoop = nRow * nCol;
	//if (nMaxLoop > MAX_CANDIDATE_AREA)	nMaxLoop = MAX_CANDIDATE_AREA;

	for (m = 0; m < 3; m++)
	{
		if (m == 0)	// 백점
		{
			candiTemp.Copy(_CandiW);
			_CandiW.Reset();
			pCandi = &_CandiW;
		}
		else if (m == 1)	// 흑점
		{
			candiTemp.Copy(_CandiB);
			_CandiB.Reset();
			pCandi = &_CandiB;
		}
		else if (m == 2)	// 찍힘
		{
			candiTemp.Copy(_CandiArea);
			_CandiArea.Reset();
			pCandi = &_CandiArea;
		}

		for (k = 0; k < nMaxLoop; k++)
		{

			nMax = 0;
			for (i = 0; i < nRow; i++)
				for (j = 0; j < nCol; j++)
				{
					if (m == 2)  //면적으로 하는 것은  l_CandiTemp.m_nCandiAvg[nNPitch*i+j] 사용하지 않음.
					{
						nTmp = candiTemp.Value[nNPitch * i + j];
					}
					else
					{
						nTmp = candiTemp.Value[nNPitch * i + j] - candiTemp.Avg[nNPitch * i + j];
						if (nTmp < 0) nTmp *= -1;
					}

					if (nTmp > nMax)
					{
						nMax = nTmp;
						nPos = nNPitch * i + j;
					}
				}

			//중복 제외----------------------------------------------------------------------
			if (nMax > 0)
			{
				nOverlap = 0;

				nPosX1 = (int)((_pParam->Cam.SttPosX + candiTemp.PosX[nPos] * _pParam->Cam.ScaleX) / MARKING_X_OFFSET);
				for (i = 0; i < k; i++)
				{
					nPosX2 = (int)((_pParam->Cam.SttPosX + pCandi->PosX[i] * _pParam->Cam.ScaleX) / MARKING_X_OFFSET);
					if (nPosX1 == nPosX2 && (int)(pCandi->PosY[i] / nLimitY) == (int)(candiTemp.PosY[nPos] / nLimitY))
					{
						nOverlap = 1;
						break;
					}
				}
				if (nOverlap)
				{
					candiTemp.Value[nPos] = candiTemp.Avg[nPos];
					continue;
				}
			}
			//--------------------------------------------------------------------------------

			if (nMax > 0)
			{
				pCandi->AddData(candiTemp.Value[nPos], candiTemp.PosX[nPos], candiTemp.PosY[nPos], candiTemp.Avg[nPos]);
				if (pCandi->Count >= _pSystem->MaxCandiDefect) break;

				candiTemp.Value[nPos] = candiTemp.Avg[nPos];
			}
		}
	}

	return 0;
}

int Inspect::CheckBoundaryOpticOverKill(unsigned char* fm, int nX, int nY, int pitch, int nGrabHeight, int Value)
{
	int i, j, ii, jj;
	unsigned char fmS[BAD_IMG_WIDTH / 4 * BAD_IMG_WIDTH / 4];
	int nOverKill = 1;
	int lPitch = BAD_IMG_WIDTH / 4;
	int nSum, nAve, nDiffThres = 14;
	int N = 0, M = 0;
	int left, top, right, bottom;
	int  nHCount = 0;
	int nMax = -1, nMin = 256, nMaxPosX, nMaxPosY, nMinPosX, nMinPosY;
	int nMaxA[4], nMinA[4];
	int nVDiffMax, nVDiffMin, nDiff, nDiffMax;
	int Offset;
	int nHMin, nHMax;
	int nLineMax = 0, nLineMin = 255;
	int nProfile_20[128];
	int nSum_20 = 0, nSum_40 = 0;
	int nSum_20_Max = 0, nBlobCount = 0;
	int nNotOverKill = 0, nRealOverKill = 0;
	int nUpper = 0, nUnder = 0, nRetry = 0;
	int nHCount1 = 0, nHMax1, nHMin1;
	int nUPCnt = 0;
	int nDNCnt = 0;
	int nCenterUpCnt = 0;
	int nDiffOffset = 3;
	int nTmp;


	left = nX - BAD_IMG_WIDTH / 2; if (left < 0) left = 0; if (left + BAD_IMG_WIDTH >= pitch) left = pitch - BAD_IMG_WIDTH;
	right = left + BAD_IMG_WIDTH;
	top = nY - BAD_IMG_HEIGHT / 2; if (top < 0) top = 0;  if ((top + BAD_IMG_HEIGHT) >= nGrabHeight) top = nGrabHeight - BAD_IMG_HEIGHT;
	bottom = top + BAD_IMG_HEIGHT;


	for (i = top; i < bottom - 1; i += 4)
	{
		M = 0;
		for (j = left; j < right - 1; j += 4)
		{
			nSum = 0;
			for (ii = i; ii < i + 4; ii++)
			{
				for (jj = j; jj < j + 4; jj++)
				{
					nSum += *(fm + pitch * ii + jj);
				}
			}
			*(fmS + N * lPitch + M) = nSum / 16;
			M++;
		}
		N++;
	}
	M--;

	nSum = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			nSum += *(fmS + i * lPitch + j);
	}
	nAve = nSum / (N * M);
	if (nAve <= 80)  nDiffThres = 14;
	else			nDiffThres = 14 + ((nAve - 80) + 6) / 12;

	//---------------------------------------------------------------------
	//수평성분만 있는 이미지는 과검으로 처리
	double dMaxArea = 0, dArea;
	memset(_fmBin, 0x00, sizeof(unsigned char) * BAD_IMG_HEIGHT * BAD_IMG_WIDTH);
	for (i = 1; i < N - 1; i++)
	{
		for (j = 0; j < M - 3; j++)
		{
			nDiff = *(fmS + i * lPitch + j) + *(fmS + (i - 1) * lPitch + j) + *(fmS + (i + 1) * lPitch + j) - *(fmS + i * lPitch + j + 3) - *(fmS + (i - 1) * lPitch + j + 3) - *(fmS + (i + 1) * lPitch + j + 3);
			if (nDiff > nDiffThres)		*(_fmBin + lPitch * i + j) = 255;
			//else						*(_fmBin + lPitch * i + j) = 0;
		}
	}

	_pChain->SetChainData(1, _fmBin, 1, 1, 1, 200, lPitch, N - 1);
	nBlobCount = _pChain->FastChain(1, 1, M - 4, N - 1);
	for (i = 0; i < nBlobCount; i++)
	{
		dArea = _pChain->Chain_Area(i);
		if (dArea > dMaxArea) dMaxArea = dArea;
	}
	if (dMaxArea < 3)
		return nOverKill;
	//--------------------------------------------------------------------


	//수평라인이면 과검------------------------------------
	nVDiffMax = 0;
	nVDiffMin = 255;
	for (j = 0; j < M; j++)
	{
		nDiffMax = 0;
		nHMax1 = 0;
		nHMin1 = 255;
		for (i = 0; i < N - 8; i++)
		{
			nDiff = *(fmS + i * lPitch + j) - *(fmS + (i + 8) * lPitch + j);
			if (nDiff < 0) nDiff *= -1;
			if (nDiff > nDiffMax) nDiffMax = nDiff;
		}
		for (i = 0; i < N; i++)
		{
			if (*(fmS + i * lPitch + j) > nHMax1) nHMax1 = *(fmS + i * lPitch + j);
			if (*(fmS + i * lPitch + j) < nHMin1) nHMin1 = *(fmS + i * lPitch + j);
		}
		if (nDiffMax > nVDiffMax) nVDiffMax = nDiffMax;
		if (nDiffMax < nVDiffMin) nVDiffMin = nDiffMax;
		if (nHMax1 - nHMin1 > 30 && nHMax1 - nHMin1 < 100) nHCount1++;
		if (nHMax1 >= 200)
			break;
	}
	if ((nVDiffMax - nVDiffMin) < 10 || nHCount1 >= M * 0.9)
	{
		if (nVDiffMax < 150 && nHMax1 < 200)
			return nOverKill;
	}
	//----------------------------------------------------

	nHMin = 255;
	nHMax = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 1; j < M - 1; j++)
		{
			if (*(fmS + i * lPitch + j) > nMax)
			{
				nMax = *(fmS + i * lPitch + j);
				nMaxPosX = j;
				nMaxPosY = i;
			}
			else if (*(fmS + i * lPitch + j) < nMin)
			{
				nMin = *(fmS + i * lPitch + j);
				nMinPosX = j;
				nMinPosY = i;
			}
			if (*(fmS + i * lPitch + j) > nHMax) nHMax = *(fmS + i * lPitch + j);
			if (*(fmS + i * lPitch + j) < nHMin) nHMin = *(fmS + i * lPitch + j);
		}
	}

	Offset = 4;
	if (nMaxPosX - Offset > 0) nMaxA[0] = *(fmS + nMaxPosY * lPitch + nMaxPosX - Offset);
	else                  nMaxA[0] = *(fmS + nMaxPosY * lPitch);

	if (nMaxPosX + Offset < M) nMaxA[2] = *(fmS + nMaxPosY * lPitch + nMaxPosX + Offset);
	else                  nMaxA[2] = *(fmS + nMaxPosY * lPitch + M - 1);

	if (nMinPosX - Offset > 0) nMinA[0] = *(fmS + nMinPosY * lPitch + nMinPosX - Offset);
	else                  nMinA[0] = *(fmS + nMinPosY * lPitch);

	if (nMinPosX + Offset < M) nMinA[2] = *(fmS + nMinPosY * lPitch + nMinPosX + Offset);
	else                  nMinA[2] = *(fmS + nMinPosY * lPitch + M - 1);

	Offset = 7;
	if (nMaxPosX - Offset > 0) nMaxA[1] = *(fmS + nMaxPosY * lPitch + nMaxPosX - Offset);
	else                  nMaxA[1] = *(fmS + nMaxPosY * lPitch);

	if (nMaxPosX + Offset < M) nMaxA[3] = *(fmS + nMaxPosY * lPitch + nMaxPosX + Offset);
	else                  nMaxA[3] = *(fmS + nMaxPosY * lPitch + M - 1);

	if (nMinPosX - Offset > 0) nMinA[1] = *(fmS + nMinPosY * lPitch + nMinPosX - Offset);
	else                  nMinA[1] = *(fmS + nMinPosY * lPitch);

	if (nMinPosX + Offset < M) nMinA[3] = *(fmS + nMinPosY * lPitch + nMinPosX + Offset);
	else                  nMinA[3] = *(fmS + nMinPosY * lPitch + M - 1);

	if ((nMax >= nMaxA[0] + Value || nMax >= nMaxA[1] + Value) && (nMax >= nMaxA[2] + Value || nMax >= nMaxA[3] + Value))
		nNotOverKill = 1;
	if ((nMin <= nMinA[0] - Value || nMin <= nMinA[1] - Value) && (nMin <= nMinA[2] - Value || nMin <= nMinA[3] - Value))
		nNotOverKill = 1;

done:

	if (nRealOverKill == 0)
	{
		double dArea, dAreaMax = 0;
		int nBlobPos;

		nOverKill = 1;
		memset(nProfile_20, 0, 128 * sizeof(int));
		memset(_fmBin, 0x00, sizeof(unsigned char) * BAD_IMG_HEIGHT * BAD_IMG_WIDTH);
		for (i = top + 1; i < bottom - 1; i++)
		{
			for (j = left + 1; j < right - 1; j++)
			{
				nTmp = *(fm + pitch * (i + 1) + j - 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1) - *(fm + pitch * (i - 1) + j - 1) - *(fm + pitch * (i - 1) + j) - *(fm + pitch * (i - 1) + j + 1);
				if (nTmp < 0) nTmp *= -1;

				if (nTmp >= 20) { *(_fmBin + BAD_IMG_WIDTH * (i - top) + (j - left)) = 255; nSum_20++; }
				//else			 *(_fmBin + BAD_IMG_WIDTH * (i - top) + (j - left)) = 0;

				if (nTmp >= 40) nSum_40++;

				nTmp = *(fm + pitch * i + j);
				if (nTmp >= 100) nUpper++;
				if (nTmp <= 50) nUnder++;
			}
		}

		_pChain->SetChainData(1, _fmBin, 1, 1, 1, 200, BAD_IMG_WIDTH, BAD_IMG_WIDTH);
		nBlobCount = _pChain->FastChain(5, 5, BAD_IMG_WIDTH - 5, BAD_IMG_WIDTH - 5);

		memset(_fmBin, 0x00, sizeof(unsigned char) * BAD_IMG_HEIGHT * BAD_IMG_WIDTH);
		if (nBlobCount >= 100)
		{
			for (i = top + 1; i < bottom - 1; i++)
			{
				for (j = left + 1; j < right - 1; j++)
				{
					nTmp = *(fm + pitch * (i + 1) + j - 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1) - *(fm + pitch * (i - 1) + j - 1) - *(fm + pitch * (i - 1) + j) - *(fm + pitch * (i - 1) + j + 1);
					if (nTmp < 0) nTmp *= -1;

					if (nTmp >= 27) *(_fmBin + BAD_IMG_WIDTH * (i - top) + (j - left)) = 255;
					//else			*(_fmBin + BAD_IMG_WIDTH * (i - top) + (j - left)) = 0;
				}
			}
			_pChain->SetChainData(1, _fmBin, 1, 1, 1, 200, BAD_IMG_WIDTH, BAD_IMG_WIDTH);
			nBlobCount = _pChain->FastChain(5, 5, BAD_IMG_WIDTH - 5, BAD_IMG_WIDTH - 5);
			nRetry = 1;
		}

		if (nBlobCount)
		{
			for (i = 0; i < nBlobCount; i++)
			{
				dArea = _pChain->Chain_Area(i);
				if (dArea > dAreaMax)
				{
					dAreaMax = dArea;
					nBlobPos = i;
				}
			}
			if (dAreaMax >= 15) nOverKill = 0;
		}
		if (nSum_40 > 4 + nRetry * 10)
		{
			nOverKill = 0;
		}

		if (nUnder > BAD_IMG_WIDTH * BAD_IMG_WIDTH * 0.1 && nSum_40 <= 4)
		{
			nNotOverKill = 0;
			nOverKill = 1;
		}
	}

	if (nNotOverKill)
		nOverKill = 0;

	return nOverKill;
}

int Inspect::CheckSimpleGradient(int nCenterX, int nCenterY, int X1, int X2, int nBottom, double dLimitvalue)
{
	int nHEIGHT = _pSystem->ImageH;
	int i, j, k, ii, jj;
	int nData[USE_CHECK_SIZE * USE_CHECK_SIZE];
	int* pData1, * pData2;
	int ll, tt, rr, bb;
	int ll_, tt_, rr_, bb_;
	int nSum, nTmp, nMax = 0;
	int nCol = 0, nRow = 0;
	unsigned char* fm = _pPyramid->GetImagePt(2);	//(1/4) 압축영상에서...
	int pitch = _pPyramid->GetImageWidth(2);
	int X1Limit = X1 / 4;
	int X2Limit = X2 / 4;
	int Y2Limit = nBottom / 4;
	int nCx = nCenterX / 4;
	int nCy = nCenterY / 4;
	int Va, Vb, Vc, Vd;

	if (X2 - X1 < USE_CHECK_SIZE * 8) return 0;


	ll = nCx - USE_CHECK_SIZE;    if (ll < X1Limit)   ll = X1Limit;
	rr = ll + USE_CHECK_SIZE * 2;  if (rr > X2Limit) { rr = X2Limit; ll = rr - USE_CHECK_SIZE * 2; }

	tt = nCy - USE_CHECK_SIZE;    if (tt < 0) tt = 0;
	bb = tt + USE_CHECK_SIZE * 2;  if (bb > Y2Limit) { bb = Y2Limit; tt = bb - USE_CHECK_SIZE * 2; }


	if (_pSystem->ImgPart == 2)
	{
		if (tt<nHEIGHT / 8 && bb>nHEIGHT / 8)
		{
			if (nCy < nHEIGHT / 8)
			{
				bb = nHEIGHT / 8;
				tt = bb - 16;
			}
			else
			{
				tt = nHEIGHT / 8;
				bb = tt + 16;
			}
		}
	}

	ll_ = ll;
	tt_ = tt;
	rr_ = rr;
	bb_ = bb;
	for (k = 0; k < 4; k++)
	{
		ll = ll_;
		tt = tt_;
		rr = rr_;
		bb = bb_;
		if (k / 2 == 1)
		{
			if (nCx <= (X1 + X2) / 2)
			{
				ll = ll_ + 1;	rr = rr_ + 1;
			}
			else
			{
				ll = ll_ - 1;	rr = rr_ + 1;
			}
		}
		if (k % 2 == 1)
		{
			if (tt_ < nHEIGHT / 8 || (tt_ >= nHEIGHT / 4 && tt_ < nHEIGHT * 3 / 8))
			{
				tt = tt_ + 1;	bb = bb_ + 1;
			}
			else
			{
				tt = tt_ - 1;	bb = bb_ - 1;
			}
		}

		nRow = 0;
		for (i = tt; i < bb - 1; i += 2)
		{
			nCol = 0;
			for (j = ll; j < rr - 1; j += 2)
			{
				nSum = 0;
				for (ii = i; ii < i + 2; ii++)
					for (jj = j; jj < j + 2; jj++)
						nSum += *(fm + ii * pitch + jj);

				*(nData + nRow * USE_CHECK_SIZE + nCol) = nSum;

				nCol++;
			}
			nRow++;
		}

		//옆에비교
		for (i = 0; i < USE_CHECK_SIZE - 1; i++)
		{
			pData1 = nData + i * USE_CHECK_SIZE;
			pData2 = nData + (i + 1) * USE_CHECK_SIZE;

			Va = *(pData1++);
			Vb = *(pData2++);

			for (j = 0; j < USE_CHECK_SIZE - 1; j++)
			{
				Vc = *(pData1++);
				Vd = *(pData2++);
				nTmp = Va - Vb; if (nTmp < 0) nTmp *= -1; if (nTmp > nMax) nMax = nTmp;
				nTmp = Va - Vc; if (nTmp < 0) nTmp *= -1; if (nTmp > nMax) nMax = nTmp;
				nTmp = Va - Vd; if (nTmp < 0) nTmp *= -1; if (nTmp > nMax) nMax = nTmp;
				nTmp = Vb - Vc; if (nTmp < 0) nTmp *= -1; if (nTmp > nMax) nMax = nTmp;
				Va = Vc;
				Vb = Vd;
			}
		}
		//맨 아랫줄
		pData1 = nData + (USE_CHECK_SIZE - 1) * USE_CHECK_SIZE;
		Va = *(pData1++);
		for (j = 0; j < USE_CHECK_SIZE - 1; j++)
		{
			Vc = *(pData1++);
			nTmp = Va - Vc; if (nTmp < 0) nTmp *= -1;
			if (nTmp > nMax) nMax = nTmp;
			Va = Vc;
		}

		//맨 오른쪽 줄
		Va = *(nData + i * USE_CHECK_SIZE + USE_CHECK_SIZE - 1);
		for (i = 0; i < USE_CHECK_SIZE - 1; i++)
		{
			Vb = *(nData + (i + 1) * USE_CHECK_SIZE + USE_CHECK_SIZE - 1);
			nTmp = Va - Vb; if (nTmp < 0) nTmp *= -1;
			if (nTmp > nMax) nMax = nTmp;
			Va = Vb;
		}

		if (nMax / 4.0 >= dLimitvalue) return 0;
	}

	return 1;
}


void Inspect::MakeDefectRect(int nDefect, int nX, int nY, int nLeft, int nRight)
{
	int ll, rr, tt, bb;

	if (nDefect<0 || nDefect>_pSystem->MaxDefect) return;

	ll = nX - BAD_IMG_WIDTH / 2;
	rr = ll + BAD_IMG_WIDTH;

	tt = nY - BAD_IMG_HEIGHT / 2;
	bb = tt + BAD_IMG_HEIGHT;

	if (ll < nLeft)
	{
		ll = nLeft; rr = ll + BAD_IMG_WIDTH;
	}
	if (rr > nRight)
	{
		rr = nRight; ll = rr - BAD_IMG_WIDTH;
	}
	if (ll < 0)
	{
		ll = 0; rr = ll + BAD_IMG_WIDTH;
	}

	if (tt < 0)
	{
		tt = 0; bb = tt + BAD_IMG_HEIGHT;
	}

	if (bb >= _pSystem->ImageH)
	{
		bb = _pSystem->ImageH - 1;
		tt = bb - BAD_IMG_HEIGHT;
	}


	_DefectData.pArea[nDefect].left = ll;
	_DefectData.pArea[nDefect].top = tt;
	_DefectData.pArea[nDefect].right = rr;
	_DefectData.pArea[nDefect].bottom = bb;
}

void Inspect::MakeDefectRect_BigDefect(int nX, int nY, int nLeft, int nRight, int* left, int* top)
{
	int ll, rr, tt, bb;


	ll = nX - 4 * BAD_IMG_WIDTH / 2;
	rr = ll + 4 * BAD_IMG_WIDTH;

	tt = nY - 4 * BAD_IMG_HEIGHT / 2;
	bb = tt + 4 * BAD_IMG_HEIGHT;

	if (ll < nLeft)
	{
		ll = nLeft; rr = ll + 4 * BAD_IMG_WIDTH;
	}
	if (rr > nRight)
	{
		rr = nRight; ll = rr - 4 * BAD_IMG_WIDTH;
	}
	if (ll < 0)
	{
		ll = 0; rr = ll + 4 * BAD_IMG_WIDTH;
	}

	if (tt < 0)
	{
		tt = 0; bb = tt + 4 * BAD_IMG_HEIGHT;
	}
	if (bb >= _pSystem->ImageH)
	{
		bb = _pSystem->ImageH - 1;
		tt = bb - 4 * BAD_IMG_HEIGHT;
	}

	*left = ll / 4;
	*top = tt / 4;
}

void Inspect::MakeDefectRect_BigDefect1(int nDefect, int nX, int nY, int nLeft, int nRight)
{
	int ll, rr, tt, bb;

	if (nDefect<0 || nDefect> _pSystem->MaxDefect) return;

	ll = nX - 4 * BAD_IMG_WIDTH / 2;
	rr = ll + 4 * BAD_IMG_WIDTH;

	tt = nY - 4 * BAD_IMG_HEIGHT / 2;
	bb = tt + 4 * BAD_IMG_HEIGHT;

	if (ll < nLeft)
	{
		ll = nLeft; rr = ll + 4 * BAD_IMG_WIDTH;
	}
	if (rr > nRight)
	{
		rr = nRight; ll = rr - 4 * BAD_IMG_WIDTH;
	}
	if (ll < 0)
	{
		ll = 0; rr = ll + 4 * BAD_IMG_WIDTH;
	}

	if (tt < 0)
	{
		tt = 0; bb = tt + 4 * BAD_IMG_HEIGHT;
	}
	if (bb >= _pSystem->ImageH)
	{
		bb = _pSystem->ImageH - 1;
		tt = bb - 4 * BAD_IMG_HEIGHT;
	}


	_DefectData.pArea[nDefect].left = ll;
	_DefectData.pArea[nDefect].top = tt;
	_DefectData.pArea[nDefect].right = rr;
	_DefectData.pArea[nDefect].bottom = bb;
}

// 이함수가 위보다 0.15msec 덜걸림(128x128)기준
int Inspect::BinaryDilate(unsigned char* fmS, unsigned char* fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
	//	int nTmp;

	int* pSumVer = new int[pitch];

	if (nSizeX % 2 == 0) nSizeX++;
	if (nSizeY % 2 == 0) nSizeY++;

	nHalfX = nSizeX / 2;
	nHalfY = nSizeY / 2;

	for (i = top; i <= bottom; i++)
		memset(fmD + pitch * i + left, 0, (right - left + 1));

	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for (i = top; i < bottom; i++)
	{
		nSum = 0;
		for (j = left; j < left + nSizeX; j++)
			nSum += *(fmS + pitch * i + j);

		pSumVer[i] = nSum;
	}
	//--------------------------------------------

	for (j = left + nHalfX; j < right - nHalfX; j++)
	{
		if (j != left + nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for (i = top; i <= bottom; i++)
				pSumVer[i] = pSumVer[i] + *(fmS + pitch * i + j - nHalfX + nSizeX - 1) - *(fmS + pitch * i + j - nHalfX - 1);
		}

		nSum = 0;
		for (i = top; i < top + nSizeY; i++)
			nSum += pSumVer[i];

		for (i = top + nHalfY; i < bottom - nHalfY; i++)
		{
			if (nSum) *(fmD + pitch * i + j) = 255;

			nSum = nSum + pSumVer[i - nHalfY + nSizeY] - pSumVer[i - nHalfY];
		}
	}

	delete[] pSumVer;

	return 0;
}


//흰점기준(흰점이 작아지게)
int Inspect::BinaryErode(unsigned char* fmS, unsigned char* fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
	//	int nTmp;
	int nStdValue;
	int* pSumVer = new int[pitch];

	if (nSizeX % 2 == 0) nSizeX++;
	if (nSizeY % 2 == 0) nSizeY++;

	nHalfX = nSizeX / 2;
	nHalfY = nSizeY / 2;


	nStdValue = 255 * nSizeX * nSizeY;

	// 초기화 빠져 있어서 초기화 추가
	for (i = top; i <= bottom; i++)
		memset(fmD + pitch * i + left, 0, (right - left + 1));

	for (i = top + nSizeY; i <= bottom - nSizeY; i++)
		memset(fmD + pitch * i + left + nSizeX, 255, (right - left + 1 - 2 * nSizeX));

	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for (i = top; i <= bottom; i++)
	{
		nSum = 0;
		for (j = left; j < left + nSizeX; j++)
			nSum += *(fmS + pitch * i + j);

		pSumVer[i] = nSum;
	}
	//--------------------------------------------

	for (j = left + nHalfX; j < right - nHalfX; j++)
	{
		if (j != left + nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for (i = top; i <= bottom; i++)
				pSumVer[i] = pSumVer[i] + *(fmS + pitch * i + j - nHalfX + nSizeX - 1) - *(fmS + pitch * i + j - nHalfX - 1);
		}

		nSum = 0;
		for (i = top; i < top + nSizeY; i++)
			nSum += pSumVer[i];

		for (i = top + nHalfY; i <= bottom - nHalfY; i++)
		{
			if (nSum != nStdValue) *(fmD + pitch * i + j) = 0;

			nSum = nSum + pSumVer[i - nHalfY + nSizeY] - pSumVer[i - nHalfY];
		}
	}

	delete[] pSumVer;

	return 0;
}


//Histogram으로 평균밝기를 구한다.--------------------------------------------------------
int Inspect::GetAveValueByHistogram(unsigned char* fm, int left, int top, int width, int height, int pitch)
{
	int i, j, nHisto[256];
	int nSum, nMax, nMaxPos;

	memset(nHisto, 0, 256 * sizeof(int));
	for (i = top; i < top + height; i++)
		for (j = left; j < left + width; j++)
			nHisto[*(fm + pitch * i + j)]++;

	nSum = nMax = nHisto[0] + nHisto[1] + nHisto[2] + nHisto[3] + nHisto[4];
	nMaxPos = 0;
	for (i = 5; i < 256; i++)
	{
		nSum = nSum + nHisto[i] - nHisto[i - 5];
		if (nSum > nMax)
		{
			nMax = nSum;
			nMaxPos = i - 4;
		}
	}
	nMax = 0;
	for (i = nMaxPos; i < nMaxPos + 5; i++)
	{
		if (nHisto[i] > nMax)
		{
			nMax = nHisto[i];
			nMaxPos = i;
		}
	}

	return nMaxPos;
}

void Inspect::GetSquireMinMax(unsigned char l1, unsigned char l2, unsigned char l3, unsigned char l4, int* pMax, int* pMin)
{
	unsigned char min = 255, max = 0;
	if (l1 < min) min = l1;
	if (l2 < min) min = l2;
	if (l3 < min) min = l3;
	if (l4 < min) min = l4;

	if (l1 > max) max = l1;
	if (l2 > max) max = l2;
	if (l3 > max) max = l3;
	if (l4 > max) max = l4;

	int val1 = (l1 + l2 + l3 + l4 - min) / 3;
	int val2 = (l1 + l2 + l3 + l4 - max) / 3;

	if (*pMax < val1) *pMax = val1;
	if (*pMin > val2) *pMin = val2;
}

//일반적인 방법으로 장변 길이를 계산
double Inspect::GetNittoSize(int nBlobNo, CRect& rt)
{
	double dCx, dCy;
	double dAngle, dPos[4][2], dShortLength, dLongLength;

	_pChain->Chain_Center(nBlobNo, &dCx, &dCy);
	dAngle = _pChain->FindAngle(nBlobNo);
	if (dAngle > 0 && dAngle <= 0.1)
		dAngle = 0.1;

	_pChain->FindMinRect(nBlobNo, dCx, dCy, dAngle, dPos);

	double a1 = abs(dPos[0][0] - dPos[1][0]) + 1;
	double a2 = abs(dPos[0][1] - dPos[1][1]) + 1;
	double b1 = abs(dPos[0][0] - dPos[3][0]) + 1;
	double b2 = abs(dPos[0][1] - dPos[3][1]) + 1;
	dLongLength = sqrt(a1 * a1 + a2 * a2);
	dShortLength = sqrt(b1 * b1 + b2 * b2);

	if (dShortLength > dLongLength)    dLongLength = dShortLength;
	if (dLongLength < 1 || dLongLength>1000)
	{
		dLongLength = 1;
		rt.left = (LONG)dCx;
		rt.right = (LONG)dCx;
		rt.top = (LONG)dCy;
		rt.bottom = (LONG)dCy;
	}
	else
	{
		dCx = (dPos[0][0] + dPos[1][0] + dPos[2][0] + dPos[3][0]) / 4;
		dCy = (dPos[0][1] + dPos[1][1] + dPos[2][1] + dPos[3][1]) / 4;

		rt.left = (LONG)(dCx - dLongLength / 2);
		rt.right = (LONG)(dCx + dLongLength / 2);
		rt.top = (LONG)(dCy - dLongLength / 2);
		rt.bottom = (LONG)(dCy + dLongLength / 2);
	}
	return dLongLength;
}

//N사 방식으로 장변 길이를 계산 (1px씩 작게 계산되며 대각선길이를 측정)
double Inspect::GetNeSize(int nBlobNo, CRect& rt)
{
	double dCx, dCy;
	double dAngle, dPos[4][2], dShortLength, dLongLength;

	_pChain->Chain_Center(nBlobNo, &dCx, &dCy);
	dAngle = _pChain->FindAngle(nBlobNo);
	if (dAngle > 0 && dAngle <= 0.1)
		dAngle = 0.1;

	_pChain->FindMinRect(nBlobNo, dCx, dCy, dAngle, dPos);

	dLongLength = sqrt((dPos[3][0] - dPos[1][0]) * (dPos[3][0] - dPos[1][0]) + (dPos[3][1] - dPos[1][1]) * (dPos[3][1] - dPos[1][1]));
	dShortLength = sqrt((dPos[0][0] - dPos[1][0]) * (dPos[0][0] - dPos[1][0]) + (dPos[0][1] - dPos[1][1]) * (dPos[0][1] - dPos[1][1]));

	if (dShortLength > dLongLength)    dLongLength = dShortLength;
	if (dLongLength < 1 || dLongLength>1000)
	{
		dLongLength = 1;
		rt.left = (LONG)dCx;
		rt.right = (LONG)dCx;
		rt.top = (LONG)dCy;
		rt.bottom = (LONG)dCy;
	}
	else
	{
		dCx = (dPos[0][0] + dPos[1][0] + dPos[2][0] + dPos[3][0]) / 4;
		dCy = (dPos[0][1] + dPos[1][1] + dPos[2][1] + dPos[3][1]) / 4;

		rt.left = (LONG)(dCx - dLongLength / 2);
		rt.right = (LONG)(dCx + dLongLength / 2);
		rt.top = (LONG)(dCy - dLongLength / 2);
		rt.bottom = (LONG)(dCy + dLongLength / 2);
	}
	return dLongLength;
}

int Inspect::GetLevel(double dValue, double* pValue)
{
	int i, nLevel = -1;

	if (dValue < 0) dValue *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (_pParam->_UseLv[i] && pValue[i] > 0 && dValue >= pValue[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

int Inspect::GetLevel(int dValue, int* pValue)
{
	int i, nLevel = -1;

	if (dValue < 0) dValue *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (_pParam->_UseLv[i] && pValue[i] > 0 && dValue >= pValue[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

int Inspect::GetLevel(double dValue, double dSize, int nAvg, int* pValue, double* pSize)
{
	int i, nLevel = -1;

	if (dValue < 0) dValue *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (_pParam->_UseLv[i] && (pValue[i] > 0.0001 || pSize[i] > 0.0001) && (int)dValue >= pValue[i] && dSize >= pSize[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

int Inspect::GetLevel(double dValueUp, double dValueDn, double dSize, int* pValueUp, int* pValueDn, double* pSize)
{
	int i, nLevel = -1;

	if (dValueUp < 0) dValueUp *= -1;
	if (dValueDn < 0) dValueDn *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (_pParam->_UseLv[i] && ((pValueUp[i] > 0.0001 && pValueDn[i] > 0.0001) || pSize[i] > 0.0001) && (int)dValueUp >= pValueUp[i] && (int)dValueDn >= pValueDn[i] && dSize >= pSize[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

int Inspect::GetLevel(double dValue, double dSize, double* pValue, double* pSize)
{
	int i, nLevel = -1;

	if (dValue < 0) dValue *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (_pParam->_UseLv[i] && (pValue[i] > 0.0001 || pSize[i] > 0.0001) && (int)dValue >= pValue[i] && dSize >= pSize[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

//m_nShadowTh
//nType=0 : White만
//nType=1 : Black만
//nType=2 : WHite Black포함

//nAveValue: 평균밝기가 주어지면 이값을 이용하고 0 이면 영역의 중간값을 이용한다.(Histo 중간값)
//nAttachPixel: 붙일 Pixel수(반드시 짝수)
//nSizeMethod: 0 LGC (가로, 세로, (가로+세로)/2), 1 (장축, 단축, 장축)   2 (장축, 단축, (장축+단축)/2
//nSizeMethod: 0 NITTO (가로, 세로, (가로+세로)/2), 1 (대각선, 가로, 세로)   2 일반적인 장축 (장축, 가로, 세로)  3 기존 N사 (대각선, 가로, 세로)

//return : *pValueUp : (최대값-평균)
//         *pValueDn : (평균값-최소)
//         *pSize    : [0]: x 또는 장축   [1]:Y 또는 단축   [2]: Size  [3]: 군집갯수 [4]:군집Size


void Inspect::GetSizeNValue(int nType, unsigned char* fm, int nDefectY, int left, int top, int width, int height, int pitch, int nAveValue, int nThUp, int nThDn, int nAttachPixel, int nSizeMethod,
	double dScaleX, double dScaleY, int* pAvg, int* pValueUp, int* pValueDn, double* pSize, double* pImulKipoSize, int* pCx, int* pCy, RECT* realRect)
{
	int i, j;
	int nAve = nAveValue;
	int nTh, nMaxValue, nMinValue;
	unsigned char fmBin[BAD_IMG_WIDTH * BAD_IMG_HEIGHT];
	unsigned char fmDilate[BAD_IMG_WIDTH * BAD_IMG_HEIGHT];
	int nBlob, nBlobID;
	int ll, tt, rr, bb, nCenterX, nCenterY = -1, nCenterXB, nCenterYB = -1;
	int nDilation = nAttachPixel / 2;
	int nSumT = 0, nSumB = 0, nUsePrev = 0;
	double dArea, dMaxArea, dist1, dist2;
	double dShadowX, dShadowY, dShadowSize;
	double dPhX, dPhY, dPhSize;
	double dSizeX, dSizeY, dSize, dGroupSize, dGroupSizeX, dGroupSizeY;
	int nGroupCount;

	InspParam::PINHOLE_PARAM* pPinhole = _pParam->_pPinhole;

	_pTmpData->UsePrevImage = 0;
	*pValueUp = *pValueUp = 0;
	pSize[0] = pSize[1] = pSize[2] = *pImulKipoSize = 0;
	*pCx = left + width / 2;
	*pCy = top + height / 2;

	if (nAttachPixel == 1) nDilation = 1; //한Pixel붙이는 걸로 되어 있으면 Dilation한번한다.

	if (width != BAD_IMG_WIDTH)  width = BAD_IMG_WIDTH;
	if (height != BAD_IMG_HEIGHT) height = BAD_IMG_HEIGHT;


	//평균밝기 다시구하기--------------------------------------------------------------
	if (_pParam->Common.AlgorithmType == COS7) //정투과는 평활화 값을 그냥사용한다.
	{
		nAve = _pSystem->FlatBright;
	}
	else
	{
		if (nAveValue == 0)
		{
			nAve = GetAveValueByHistogram(fm, left, top, width, height, pitch);

			if (nAve - _pSystem->FlatBright >= 50)			//Big White의 경우  영역평균값이 평활화 값보다 50이상 밝으면 평활화 값을 사용한다.
				nAve = _pSystem->FlatBright;
			else if (_pSystem->FlatBright - nAve >= 30)    //Big Black의 경우  영역평균값이 평활화 값보다 30이상 어두우면 평활화 값을 사용한다.
				nAve = _pSystem->FlatBright;
		}
	}

	//---------------------------------------------------------------------------------
	if (_pSystem->CamType == 1 && nDefectY < 32 && top == 0) //라인스캔이고, 불량이
	{
		bb = height - 32;
		for (j = left; j < left + width; j++)
		{
			nSumB += *(fm + pitch * bb + j);
		}
	}

	//최대 최소 구함------------------------------------------------------------------
	nMaxValue = 0;
	nMinValue = 255;
	for (i = top; i < top + height; i++)
	{
		for (j = left; j < left + width; j++)
		{
			nTh = *(fm + pitch * i + j);
			if (nTh > nMaxValue)
			{
				nMaxValue = nTh;
				nCenterX = j - left;
				nCenterY = i - top;
			}
			else if (nTh < nMinValue)
			{
				nMinValue = nTh;
				nCenterXB = j - left;
				nCenterYB = i - top;
			}
		}
	}

	*pAvg = nAve;
	*pValueUp = nMaxValue - nAve;
	*pValueDn = nAve - nMinValue;


	//Binary----------------------------------------------------------------------------
	memset(fmBin, 0, BAD_IMG_WIDTH * BAD_IMG_HEIGHT);
	if (nType == 0 || nType == 2)  //백점+찍힘
	{
		if (nType == 0)
		{
			nTh =nAve+nThUp-1;
			for (i = 0; i < BAD_IMG_HEIGHT; i++)
				for (j = 0; j < BAD_IMG_WIDTH; j++)
					if (*(fm + pitch * (i + top) + (j + left)) > nTh) *(fmBin + BAD_IMG_WIDTH * i + j) = 255;
		}
		else
		{
			nTh = nAve + nThUp - 1; //
			for (i = 0; i < BAD_IMG_HEIGHT - 1; i++)
				for (j = 0; j < BAD_IMG_WIDTH - 1; j++)
					if ((*(fm + pitch * (i + top) + (j + left)) + *(fm + pitch * (i + top) + (j + left + 1)) + *(fm + pitch * (i + top + 1) + (j + left)) + *(fm + pitch * (i + top + 1) + (j + left + 1))) / 4 > nTh)
						*(fmBin + BAD_IMG_WIDTH * i + j) = 255;

			for (i = 0; i < BAD_IMG_HEIGHT - 1; i++)
				*(fmBin + BAD_IMG_WIDTH * i + BAD_IMG_WIDTH - 1) = *(fmBin + BAD_IMG_WIDTH * i + BAD_IMG_WIDTH - 2);
			for (j = 0; j < BAD_IMG_WIDTH; j++)
				*(fmBin + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1) + j) = *(fmBin + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 2) + j);
		}
	}
	if (nType == 1 || nType == 2) //흑점+찍힘
	{
		if (nType == 1)
		{
			nTh = nAve - nThDn + 1; //
			for (i = 0; i < BAD_IMG_HEIGHT; i++)
				for (j = 0; j < BAD_IMG_WIDTH; j++)
					if (*(fm + pitch * (i + top) + (j + left)) < nTh)
						*(fmBin + BAD_IMG_WIDTH * i + j) = 255;
		}
		else
		{
			nTh = nAve - nThDn + 1; //
			for (i = 0; i < BAD_IMG_HEIGHT - 1; i++)
				for (j = 0; j < BAD_IMG_WIDTH - 1; j++)
					if ((*(fm + pitch * (i + top) + (j + left)) + *(fm + pitch * (i + top) + (j + left + 1)) + *(fm + pitch * (i + top + 1) + (j + left)) + *(fm + pitch * (i + top + 1) + (j + left + 1))) / 4 < nTh)
						*(fmBin + BAD_IMG_WIDTH * i + j) = 255;

			for (i = 0; i < BAD_IMG_HEIGHT - 1; i++)
				*(fmBin + BAD_IMG_WIDTH * i + BAD_IMG_WIDTH - 1) = *(fmBin + BAD_IMG_WIDTH * i + BAD_IMG_WIDTH - 2);
			for (j = 0; j < BAD_IMG_WIDTH; j++)
				*(fmBin + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1) + j) = *(fmBin + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 2) + j);
		}
	}

	_pTmpData->UsePrevImage = nUsePrev;
	//-----------------------------------------------------------------------------------

	if (nType == 3)	// S1 검사
	{
		int jj;
		int leftMax, rightMax;
		int val;

		nTh = nAve - nThDn + 1; //
		for (i = 0; i < BAD_IMG_HEIGHT - 1; i++)
		{
			for (j = 2; j < BAD_IMG_WIDTH - 3; j++)
			{
				leftMax = rightMax = 0;
				for (jj = 1; jj <= 2; jj++)
				{
					val = nAve - *(fm + pitch * (i + top) + (j + left - jj));
					if (val > leftMax) leftMax = val;

					val = *(fm + pitch * (i + top) + (j + left + jj)) - nAve;
					if (val > rightMax) rightMax = val;
				}
				if (leftMax > nThDn - 5 && rightMax > nThUp - 5 && leftMax + rightMax >= (nThUp + nThDn))
					*(fmBin + BAD_IMG_WIDTH * i + j) = 255;
			}
		}

		for (i = 0; i < BAD_IMG_HEIGHT - 1; i++)
			*(fmBin + BAD_IMG_WIDTH * i + BAD_IMG_WIDTH - 1) = *(fmBin + BAD_IMG_WIDTH * i + BAD_IMG_WIDTH - 2);
		for (j = 0; j < BAD_IMG_WIDTH; j++)
			*(fmBin + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1) + j) = *(fmBin + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 2) + j);
	}

	//이웃한 Pixel붙이기------------------------------------------------------------------------------------------------------------	
	if (nDilation)
	{
		if (_pParam->Common.AlgorithmType == COS2)

		{
			if (nType == 2)
			{
				BinaryDilate(fmBin, fmDilate, 0, 0, BAD_IMG_WIDTH - 1, BAD_IMG_HEIGHT - 1, BAD_IMG_WIDTH, nDilation * 2 + 1, nDilation * 3 + 1);
			}
			else
			{
				BinaryDilate(fmBin, fmDilate, 0, 0, BAD_IMG_WIDTH - 1, BAD_IMG_HEIGHT - 1, BAD_IMG_WIDTH, nDilation * 2 + 1, nDilation * 2 + 1);
				BinaryErode(fmDilate, fmBin, 0, 0, BAD_IMG_WIDTH - 1, BAD_IMG_HEIGHT - 1, BAD_IMG_WIDTH, nDilation * 2 + 1, nDilation * 2 + 1);
				memcpy(fmDilate, fmBin, BAD_IMG_WIDTH * BAD_IMG_HEIGHT);
			}
		}
		else
		{
			if (nType == 2) BinaryDilate(fmBin, fmDilate, 0, 0, BAD_IMG_WIDTH - 1, BAD_IMG_HEIGHT - 1, BAD_IMG_WIDTH, 1, nDilation * 2 + 1);
			else if (nType == 3)
			{
				BinaryDilate(fmBin, fmDilate, 0, 0, BAD_IMG_WIDTH - 1, BAD_IMG_HEIGHT - 1, BAD_IMG_WIDTH, 1, nDilation * 6 + 1);
				BinaryErode(fmDilate, fmBin, 0, 0, BAD_IMG_WIDTH - 1, BAD_IMG_HEIGHT - 1, BAD_IMG_WIDTH, 1, nDilation * 6 + 1);
				memcpy(fmDilate, fmBin, BAD_IMG_WIDTH* BAD_IMG_HEIGHT);
			}
			else		 
				BinaryDilate(fmBin, fmDilate, 0, 0, BAD_IMG_WIDTH - 1, BAD_IMG_HEIGHT - 1, BAD_IMG_WIDTH, nDilation * 2 + 1, nDilation * 2 + 1);
		}
	}
	else
		memcpy(fmDilate, fmBin, BAD_IMG_WIDTH* BAD_IMG_HEIGHT);

	_pChain->SetChainData(1, fmDilate, 1, 1, 1, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
	nBlob = _pChain->FastChain(nDilation, nDilation, BAD_IMG_WIDTH - 1 - nDilation, BAD_IMG_HEIGHT - 1 - nDilation);

	if (nBlob > 0)
	{
		int t1, b1, l1, r1;
		dMaxArea = 0;
		for (i = 0; i < nBlob; i++)
		{
			dArea = _pChain->Chain_Area(i);
			l1 = _pChain->FindMinX(i);
			r1 = _pChain->FindMaxX(i);
			t1 = _pChain->FindMinY(i);
			b1 = _pChain->FindMaxY(i);

			if (dArea >= dMaxArea)//최대 불량 사이즈.
			{
				nBlobID = i;
				dMaxArea = dArea;
			}
		}
		if (dMaxArea < 0.1) return;

		if (nType == 2)
		{
			ll = _pChain->FindMinX(nBlobID);
			rr = _pChain->FindMaxX(nBlobID);
			tt = _pChain->FindMinY(nBlobID);// + nDilation;
			bb = _pChain->FindMaxY(nBlobID);// - nDilation;

		}
		else
		{
			
			ll = _pChain->FindMinX(nBlobID);//+ nDilation;
			rr = _pChain->FindMaxX(nBlobID);//- nDilation;
			tt = _pChain->FindMinY(nBlobID);//+ nDilation;
			bb = _pChain->FindMaxY(nBlobID);//- nDilation;

			if (rr - ll > 1)
			{
				ll += nDilation;
				rr -= nDilation;
			}
		}

		if (bb - tt > 1)
		{
			tt += nDilation;
			bb -= nDilation;
		}

		// 불량 확인 표시용으로 추가
		realRect->left = ll;
		realRect->top = tt;
		realRect->right = rr;
		realRect->bottom = bb;

		CRect rt;
		pSize[0] = (rr - ll + 1) * dScaleX;
		pSize[1] = (bb - tt + 1) * dScaleY;

		if (nSizeMethod == 0)
			pSize[2] = (pSize[0] + pSize[1]) * 0.5;
		else if (nSizeMethod == 2)
			pSize[2] = GetNittoSize(nBlobID, rt) * (dScaleX + dScaleY) * 0.5;
		else if (nSizeMethod == 3)
			pSize[2] = GetNeSize(nBlobID, rt) * (dScaleX + dScaleY) * 0.5;
		else
			pSize[2] = sqrt(pSize[0] * pSize[0] + pSize[1] * pSize[1]);

		nCenterX = (ll + rr) / 2;
		nCenterY = (tt + bb) / 2;


		//SHADOW 크기체크
		if (pPinhole !=nullptr && pPinhole->TH > 0)  //핀홀 TH가 있으면 그림자 찾는다.
		{
			//0. 핀홀 관련 전역변수 초기화
			_pTmpData->Pinhole.IsLarge = 0;
			_pTmpData->Pinhole.IsShadeSkip = 0;
			_pTmpData->Pinhole.IsGroup = 0;

			if (nType == 0 && fm == _pFlatImg)
			{
				//1. 핀홀 크기를 측정
				dPhX = 0, dPhY = 0, dPhSize = 0;
				nTh = nAve + pPinhole->TH - 1; //
				memset(fmBin, 0, BAD_IMG_WIDTH* BAD_IMG_HEIGHT);

				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						if (*(fm + pitch * (i + top) + (j + left)) > nTh) *(fmBin + BAD_IMG_WIDTH * i + j) = 255;

				_pChain->SetChainData(1, fmBin, 1, 1, 1, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
				ll = ll - 1;  if (ll < 0) ll = 0;
				tt = tt - 1;  if (tt < 0) tt = 0;
				rr = rr + 1;  if (rr >= BAD_IMG_WIDTH - 1) rr = BAD_IMG_WIDTH - 1;
				bb = bb + 1;  if (bb >= BAD_IMG_HEIGHT - 1) bb = BAD_IMG_HEIGHT - 1;

				int cx = (ll + rr) / 2;
				int cy = (tt + bb) / 2;
				nBlob = _pChain->FastChain(ll, tt, rr, bb);
				if (nBlob > 0)
				{
					dMaxArea = 0;
					nBlobID = -1;
					for (i = 0; i < nBlob; i++)
					{
						dArea = _pChain->Chain_Area(i);
						if (dArea >= dMaxArea)//최대 불량 사이즈.
						{
							nBlobID = i;
							dMaxArea = dArea;
						}
					}
					if (dMaxArea < 0.1) return;

					if (nBlobID >= 0)
					{
						ll = _pChain->FindMinX(nBlobID);
						rr = _pChain->FindMaxX(nBlobID);
						tt = _pChain->FindMinY(nBlobID);
						bb = _pChain->FindMaxY(nBlobID);

						dPhX = (rr - ll + 1) * dScaleX;
						dPhY = (bb - tt + 1) * dScaleY;
						if (nSizeMethod == 0)
							dPhSize = (dPhX + dPhY) / 2;
						else if (nSizeMethod == 2)
							dPhSize = GetNittoSize(nBlobID, rt) * (dScaleX + dScaleY) * 0.5;
						else if (nSizeMethod == 3)
							dPhSize = GetNeSize(nBlobID, rt) * (dScaleX + dScaleY) * 0.5;
						else
							dPhSize = sqrt(pSize[0] * pSize[0] + pSize[1] * pSize[1]);

						_pTmpData->Pinhole.Size = dPhSize;

						// 불량 확인 표시용으로 추가
						realRect->left = ll;
						realRect->top = tt;
						realRect->right = rr;
						realRect->bottom = bb;
					}
				}

				//2. 핀홀 크기가 그림자 Skip제외 크기보다 크면, 강 불량처리
				if (dPhSize >= 0.001 && dPhSize >= pPinhole->ShadowSkipSize)
				{
					pSize[0] = dPhX;
					pSize[1] = dPhY;
					pSize[2] = dPhSize;

					nCenterX = (ll + rr) / 2;
					nCenterY = (tt + bb) / 2;
					_pTmpData->Pinhole.IsLarge = 1;

					// 불량 확인 표시용으로 추가
					realRect->left = ll;
					realRect->top = tt;
					realRect->right = rr;
					realRect->bottom = bb;

					return;
				}

				int nLevel = GetLevel(*pValueUp, pSize[2], nAve, _pParam->_pLvUp->LvTH, _pParam->_pLvUp->LvSize);

				//3. 결점의 크기가 최소조건을 만족하고, 핀홀이 검출되었고 그림자 스킵 사용하면, 체크
				if (pPinhole!=nullptr && nLevel >= 0 && dPhSize >= 0.001 && pPinhole->ShadowTH> 0 && pPinhole->ShadowTH < pPinhole->TH)
				{
					dShadowX = 0, dShadowY = 0, dShadowSize = 0;
					nTh = nAve + pPinhole->ShadowTH - 1; //
					memset(fmBin, 0, BAD_IMG_WIDTH * BAD_IMG_HEIGHT);

					for (i = 0; i < BAD_IMG_HEIGHT; i++)
						for (j = 0; j < BAD_IMG_WIDTH; j++)
							if (*(fm + pitch * (i + top) + (j + left)) > nTh) *(fmBin + BAD_IMG_WIDTH * i + j) = 255;

					_pChain->SetChainData(1, fmBin, 1, 1, 1, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
					ll = ll - pPinhole->ShadowOffset - 1;  if (ll < 0) ll = 0;
					tt = tt - pPinhole->ShadowOffset - 1;  if (tt < 0) tt = 0;
					rr = rr + pPinhole->ShadowOffset + 1;  if (rr >= BAD_IMG_WIDTH - 1) rr = BAD_IMG_WIDTH - 1;
					bb = bb + pPinhole->ShadowOffset + 1;  if (bb >= BAD_IMG_HEIGHT - 1) bb = BAD_IMG_HEIGHT - 1;
					nBlob = _pChain->FastChain(ll, tt, rr, bb);
					if (nBlob > 0)
					{
						dMaxArea = 0;
						nBlobID = -1;
						for (i = 0; i < nBlob; i++)
						{
							dArea = _pChain->Chain_Area(i);
							if (dArea >= dMaxArea)//최대 불량 사이즈.
							{
								nBlobID = i;
								dMaxArea = dArea;
							}
						}

						if (nBlobID >= 0)
						{
							ll = _pChain->FindMinX(nBlobID);
							rr = _pChain->FindMaxX(nBlobID);
							tt = _pChain->FindMinY(nBlobID);
							bb = _pChain->FindMaxY(nBlobID);

							dShadowX = (rr - ll + 1) * dScaleX;
							dShadowY = (bb - tt + 1) * dScaleY;
							if (nSizeMethod == 0)
								dShadowSize = (dShadowX + dShadowY) / 2;
							else if (nSizeMethod == 2)
								dShadowSize = GetNittoSize(nBlobID, rt) * (dScaleX + dScaleY) * 0.5;
							else if (nSizeMethod == 3)
								dShadowSize = GetNeSize(nBlobID, rt) * (dScaleX + dScaleY) * 0.5;
							else
								dShadowSize = sqrt(dShadowX * dShadowX + dShadowY * dShadowY);

							_pTmpData->Pinhole.ShadeSize = dShadowSize;

							// 불량 확인 표시용으로 추가
							realRect->left = ll;
							realRect->top = tt;
							realRect->right = rr;
							realRect->bottom = bb;
						}
					}

					if (dShadowSize >= 0.001 && pPinhole->ShadowSkipSize > 0.001)
					{
						//그림자 스킵사용시, 스킵 확인
						if (dShadowSize - dPhSize >= pPinhole->ShadowSkipSize)
							_pTmpData->Pinhole.IsShadeSkip = 1;
					}
					else
					{
						//그림자 스킵 미사용시, 그림자까지 포함해서 핀홀 크기 측정
						pSize[0] = dShadowX;
						pSize[1] = dShadowY;
						pSize[2] = dShadowSize;

						nCenterX = (ll + rr) / 2;
						nCenterY = (tt + bb) / 2;
					}
				}

				//핀홀 군집조건 체크
				if (_pParam->_pGroup->Base.TH > 0 && _pParam->_pGroup->Base.Count > 0 && _pParam->_pGroup->Base.Size > 0)
				{
					nTh = nAve + _pParam->_pGroup->Base.TH - 1; //
					memset(fmBin, 0, BAD_IMG_WIDTH * BAD_IMG_HEIGHT);

					for (i = 0; i < BAD_IMG_HEIGHT; i++)
						for (j = 0; j < BAD_IMG_WIDTH; j++)
							if (*(fm + pitch * (i + top) + (j + left)) > nTh) *(fmBin + BAD_IMG_WIDTH * i + j) = 255;


					//N사와 동일하게 기본 Binary TH의 Object영역을 Group TH의 Object영역을 OR연산 한 후 처리
					ll = cx - _pParam->_pGroup->Base.Area;
					tt = cy - _pParam->_pGroup->Base.Area;
					rr = cx + _pParam->_pGroup->Base.Area;
					bb = cy + _pParam->_pGroup->Base.Area;
					if (ll < 0) ll = 0;
					if (tt < 0) tt = 0;
					if (rr > BAD_IMG_WIDTH) rr = BAD_IMG_WIDTH;
					if (bb > BAD_IMG_HEIGHT) bb = BAD_IMG_HEIGHT;
					for (i = 0; i < BAD_IMG_HEIGHT; i++)
						for (j = 0; j < BAD_IMG_WIDTH; j++)
							if (j < ll || j >= rr || i < tt || i >= bb)
								*(fmBin + BAD_IMG_WIDTH * i + j) = 0;

					_pChain->SetChainData(1, fmBin, 1, 1, 1, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
					nBlob = _pChain->FastChain(0, 0, BAD_IMG_WIDTH - 1, BAD_IMG_HEIGHT - 1);

					int groupLeft = BAD_IMG_WIDTH;
					int groupTop = BAD_IMG_HEIGHT;
					int groupRight = 0;
					int groupBottom = 0;
					nGroupCount = 0; dGroupSize = 0; dGroupSizeX = 0; dGroupSizeY = 0;
					for (i = 0; i < nBlob; i++)
					{
						dArea = _pChain->Chain_Area(i);
						if (dArea < 0) continue;

						ll = _pChain->FindMinX(i);
						rr = _pChain->FindMaxX(i);
						tt = _pChain->FindMinY(i);
						bb = _pChain->FindMaxY(i);

						dSizeX = (rr - ll + 1) * dScaleX;
						dSizeY = (bb - tt + 1) * dScaleY;
						dSize = (dSizeX + dSizeY) / 2;

						if (nSizeMethod == 0)
							dSize = (dSizeX + dSizeY) / 2;
						else if (nSizeMethod == 2)
							dSize = GetNittoSize(i, rt) * (dScaleX + dScaleY) * 0.5;
						else if (nSizeMethod == 3)
							dSize = GetNeSize(i, rt) * (dScaleX + dScaleY) * 0.5;
						else
							dSize = sqrt(pSize[0] * pSize[0] + pSize[1] * pSize[1]);

						if (dSize < _pParam->_pGroup->Base.Size) continue;

						dist1 = nCenterX - (ll + rr) / 2; if (dist1 < 0) dist1 *= -1;
						dist2 = nCenterY - (tt + bb) / 2; if (dist2 < 0) dist2 *= -1;

						//거리체크는 N사와 동일하게 Blob Center기준으로 적용 (요구사항과 다름)
						// 0.001 추가는 연산 오차로 생기는 부분 보상
						// ex) th는 0.01이나 계산 오차는 0.099999 나오는 경우 보상 처리
						if ((sqrt(dist1 * dist1 + dist2 * dist2) + 0.001) <= _pParam->_pGroup->Base.Area)
						{
							nGroupCount++;
							dGroupSize += dSize;
							dGroupSizeX += dSizeX;
							dGroupSizeY += dSizeY;

							
							groupLeft = __min(groupLeft, ll);
							groupRight = __max(groupRight, rr);
							groupTop = __min(groupTop, tt);
							groupBottom = __max(groupBottom, bb);
						}
					}
					if (nGroupCount >= _pParam->_pGroup->Base.Count)
					{
						_pTmpData->Pinhole.IsShadeSkip = 0;	//군집이면, 그림자스킵 무시
						_pTmpData->Pinhole.IsGroup = 1;		//군집결점으로 마크

						//군집판정시 Size를 키워서 적용
						pSize[0] = dGroupSizeX;
						pSize[1] = dGroupSizeY;
						pSize[2] = dGroupSize;
						pSize[3] = nGroupCount;
						pSize[4] = dGroupSize;

						// 불량 확인 표시용으로 추가
						realRect->left = groupLeft;
						realRect->top = groupTop;
						realRect->right = groupRight;
						realRect->bottom = groupBottom;
					}
				}
			}
		}

		//찍힘이면 Value 다시 구함------------------------------------------------------------------------
		if (nType == 2 || nType == 3)
		{
			ll -= 2; if (ll < 0) ll = 0;
			tt -= 2; if (tt < 0) tt = 0;
			rr += 2; if (rr >= BAD_IMG_WIDTH - 1) rr = BAD_IMG_WIDTH - 2;
			bb += 2; if (bb >= BAD_IMG_HEIGHT - 1) bb = BAD_IMG_HEIGHT - 2;

			nMaxValue = 0;
			nMinValue = 255;

			for (i = top + tt; i < top + bb; i++)
				for (j = left + ll; j < left + rr; j++)
				{
					GetSquireMinMax(*(fm + pitch * i + j), *(fm + pitch * i + j + 1), *(fm + pitch * (i + 1) + j), *(fm + pitch * (i + 1) + j + 1), &nMaxValue, &nMinValue);
				}

			*pValueUp = nMaxValue - nAve;
			*pValueDn = nAve - nMinValue;

			// 가로로 변화가 심한지 세로로 변화가 심한지 확인함. 
			// 변화량에 따라 찍힘 혹은 주름 불량 확인

		}
		//------------------------------------------------------------------------------------------------	
		*pCx = left + (ll + rr) / 2;
		*pCy = top + (tt + bb) / 2;
	}
	//-------------------------------------------------------------------------------------------------------
}


//2개의 Level과 Class를 받아서 더 중요한 것을 Return 한다.
//조건이 같으면 앞에것 선택
int Inspect::SelectDefect(int nLevel1, int nClass1, int nLevel2, int nClass2)
{
	int nClass = 0;

	int pcFirstNo = _pTmpData->PcInfo.FirstNo;

	//데이타가 정상이 아니면 정상인 데이타를  Return 하든지 0을 Return한다------------------------------------------
	if (nLevel1 < 1 || nLevel1>10 || nClass1 < 1 || nClass1>9 || nLevel2 < 1 || nLevel2>10 || nClass2 < 1 || nClass2>9)
	{
		if (!(nLevel1 < 1 || nLevel1>10 || nClass1 < 1 || nClass1>9))
		{
			nClass = MILLION * nLevel1 + pcFirstNo * CLASSDEV + nClass1;
		}
		if (!(nLevel2 < 1 || nLevel2>10 || nClass2 < 1 || nClass2>9))
		{
			nClass = MILLION * nLevel2 + pcFirstNo * CLASSDEV + nClass2;
		}
		return nClass;
	}
	//--------------------------------------------------------------------------------------------------------------


	if (_pMark->Marking[nClass1 - 1][nLevel1 - 1] == _pMark->Marking[nClass2 - 1][nLevel2 - 1])  //마킹여부가 같다면
	{
		if (nLevel1 == nLevel2) //같은 마킹, 같은 Level이면 앞에 있는 것으로 선택
		{
			nClass = MILLION * nLevel1 + pcFirstNo * CLASSDEV + nClass1;
		}
		else
		{
			if (nLevel1 < nLevel2) nClass = MILLION * nLevel1 + pcFirstNo * CLASSDEV + nClass1;
			else				nClass = MILLION * nLevel2 + pcFirstNo * CLASSDEV + nClass2;
		}
	}
	else //하나는 마킹하고 하나는 마킹하지 않으면 마킹우선
	{
		if (_pMark->Marking[nClass1 - 1][nLevel1 - 1])
		{
			nClass = MILLION * nLevel1 + pcFirstNo * CLASSDEV + nClass1;
		}
		else
		{
			nClass = MILLION * nLevel2 + pcFirstNo * CLASSDEV + nClass2;
		}
	}

	return nClass;
}


//bFlat=true : 평활화 영상
//     =false: 원본영상
int Inspect::CopyNGImage(bool bFlat)
{
	int i, j, nTmp;
	int nOffX = _DefectData.pArea[_DefectData.Count].left;
	int nOffY = _DefectData.pArea[_DefectData.Count].top;
	int pitch = _pSystem->Pitch;
	LPBYTE fm;
	LPBYTE fmD = _DefectData.pImage[_DefectData.Count];

	if (bFlat)
	{
		fm = _pFlatImg;
	}
	else
	{
		fm = _pFlatImg;
		_pTmpData->UsePrevImage = 0;
	}

	if (_pSystem->CamType == 0 && _pSystem->ImgPart > 1)
	{
		nTmp = (nOffY + BAD_IMG_HEIGHT / 2) / _pSystem->ImgPartH;
		if (nOffY < nTmp * _pSystem->ImgPartH) { nOffY = nTmp * _pSystem->ImgPartH; }
		else if (nOffY + BAD_IMG_HEIGHT >= (nTmp + 1) * _pSystem->ImgPartH) { nOffY = (nTmp + 1) * _pSystem->ImgPartH - BAD_IMG_HEIGHT; }
	}

	if (_pSystem->UseImgFlap && _pSystem->UseImgMirror)
	{
		/*if (_pSystem->ApplyLUT2Defect)
		{
			if (_pTmpData->UsePrevImage)
			{
				for (i = 0; i < 32; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + (BAD_IMG_WIDTH - 1 - j)) = g_DefectLUT[*(g_fmPrev + pitch * i + j + nOffX)];
				for (i = 32; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + (BAD_IMG_WIDTH - 1 - j)) = g_DefectLUT[*(fm + pitch * (i + nOffY - 32) + j + nOffX)];
			}
			else
			{
				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + (BAD_IMG_WIDTH - 1 - j)) = g_DefectLUT[*(fm + pitch * (i + nOffY) + j + nOffX)];
			}
		}
		else*/
		{
			/*if (_pTmpData->UsePrevImage)
			{
				for (i = 0; i < 32; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + (BAD_IMG_WIDTH - 1 - j)) = *(g_fmPrev + pitch * i + j + nOffX);
				for (i = 32; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + (BAD_IMG_WIDTH - 1 - j)) = *(fm + pitch * (i + nOffY - 32) + j + nOffX);
			}
			else*/
			{
				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + (BAD_IMG_WIDTH - 1 - j)) = *(fm + pitch * (i + nOffY) + j + nOffX);
			}
		}
	}
	else if (_pSystem->UseImgFlap && !_pSystem->UseImgMirror)
	{
		/*if (_pSystem->ApplyLUT2Defect)
		{
			if (_pTmpData->UsePrevImage)
			{
				for (i = 0; i < 32; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + j) = g_DefectLUT[*(g_fmPrev + pitch * i + j + nOffX)];
				for (i = 32; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + j) = g_DefectLUT[*(fm + pitch * (i + nOffY - 32) + j + nOffX)];
			}
			else
			{
				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i) + j) = g_DefectLUT[*(fm + pitch * (i + nOffY) + j + nOffX)];
			}
		}
		else*/
		{
			/*if (_pTmpData->UsePrevImage)
			{
				for (i = 0; i < 32; i++)
					memcpy(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i), g_fmPrev + pitch * i + nOffX, sizeof(byte) * BAD_IMG_WIDTH);
				for (i = 32; i < BAD_IMG_HEIGHT; i++)
					memcpy(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i), fm + pitch * (i + nOffY - 32) + nOffX, sizeof(byte) * BAD_IMG_WIDTH);
			}
			else*/
			{
				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					memcpy(fmD + BAD_IMG_WIDTH * (BAD_IMG_HEIGHT - 1 - i), fm + pitch * (i + nOffY) + nOffX, sizeof(unsigned char) * BAD_IMG_WIDTH);
			}
		}
	}
	else if (!_pSystem->UseImgFlap && _pSystem->UseImgMirror)
	{
		/*if (_pSystem->ApplyLUT2Defect)
		{
			if (_pTmpData->UsePrevImage)
			{
				for (i = 0; i < 32; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + (BAD_IMG_WIDTH - 1 - j)) = g_DefectLUT[*(g_fmPrev + pitch * i + j + nOffX)];
				for (i = 32; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + (BAD_IMG_WIDTH - 1 - j)) = g_DefectLUT[*(fm + pitch * (i + nOffY - 32) + j + nOffX)];
			}
			else
			{
				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + (BAD_IMG_WIDTH - 1 - j)) = g_DefectLUT[*(fm + pitch * (i + nOffY) + j + nOffX)];
			}
		}
		else*/
		{
			/*if (_pTmpData->UsePrevImage)
			{
				for (i = 0; i < 32; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + (BAD_IMG_WIDTH - 1 - j)) = *(g_fmPrev + pitch * i + j + nOffX);

				for (i = 32; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + (BAD_IMG_WIDTH - 1 - j)) = *(fm + pitch * (i + nOffY - 32) + j + nOffX);
			}
			else*/
			{
				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + (BAD_IMG_WIDTH - 1 - j)) = *(fm + pitch * (i + nOffY) + j + nOffX);
			}
		}
	}
	else
	{
		/*if (_pSystem->ApplyLUT2Defect)
		{
			if (_pTmpData->UsePrevImage)
			{
				for (i = 0; i < 32; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + j) = g_DefectLUT[*(g_fmPrev + pitch * i + j + nOffX)];
				for (i = 32; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + j) = g_DefectLUT[*(fm + pitch * (i + nOffY - 32) + j + nOffX)];
			}
			else
			{
				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + j) = g_DefectLUT[*(fm + pitch * (i + nOffY) + j + nOffX)];
			}
		}
		else*/
		{
			/*if (_pTmpData->UsePrevImage)
			{
				for (i = 0; i < 32; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + j) = *(g_fmPrev + pitch * i + j + nOffX);

				for (i = 32; i < BAD_IMG_HEIGHT; i++)
					for (j = 0; j < BAD_IMG_WIDTH; j++)
						*(fmD + BAD_IMG_WIDTH * i + j) = *(fm + pitch * (i + nOffY - 32) + j + nOffX);
			}
			else*/
			{
				for (i = 0; i < BAD_IMG_HEIGHT; i++)
					memcpy(fmD + BAD_IMG_WIDTH * i, fm + pitch * (i + nOffY) + nOffX, sizeof(unsigned char) * BAD_IMG_WIDTH);
			}
		}
	}

	return 0;
}

int Inspect::CopyNGImageTemp(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch)
{
	int i, j;
	int nOffX, nOffY;

	/*if (_pSystem->ApplyLUT2Defect)
	{
		for (i = 0; i < BAD_IMG_HEIGHT; i++)
		{
			nOffY = top + (bottom - top) * i / BAD_IMG_HEIGHT;
			for (j = 0; j < BAD_IMG_WIDTH; j++)
			{
				nOffX = left + (right - left) * j / BAD_IMG_WIDTH;
				*(fmD + BAD_IMG_WIDTH * i + j) = g_DefectLUT[*(fm + pitch * nOffY + nOffX)];
			}
		}
	}
	else*/
	{
		for (i = 0; i < BAD_IMG_HEIGHT; i++)
		{
			nOffY = top + (bottom - top) * i / BAD_IMG_HEIGHT;
			for (j = 0; j < BAD_IMG_WIDTH; j++)
			{
				nOffX = left + (right - left) * j / BAD_IMG_WIDTH;
				*(fmD + BAD_IMG_WIDTH * i + j) = *(fm + pitch * nOffY + nOffX);
			}
		}
	}

	return 0;
}

//(0,0)에 Gray로 표시
int Inspect::CopyNGImageFMArea(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGray)
{
	int i, j;
	int nOffX, nOffY;
	LPBYTE fmD = _DefectData.pImage[_DefectData.Count];

	/*if (_pSystem->ApplyLUT2Defect)
	{
		for (i = 0; i < BAD_IMG_HEIGHT; i++)
		{
			nOffY = top + (bottom - top) * i / BAD_IMG_HEIGHT;
			for (j = 0; j < BAD_IMG_WIDTH; j++)
			{
				nOffX = left + (right - left) * j / BAD_IMG_WIDTH;
				*(fmD + BAD_IMG_WIDTH * i + j) = g_DefectLUT[*(fm + pitch * nOffY + nOffX)];
			}
		}
	}
	else*/
	{
		for (i = 0; i < BAD_IMG_HEIGHT; i++)
		{
			nOffY = top + (bottom - top) * i / BAD_IMG_HEIGHT;
			for (j = 0; j < BAD_IMG_WIDTH; j++)
			{
				nOffX = left + (right - left) * j / BAD_IMG_WIDTH;
				*(fmD + BAD_IMG_WIDTH * i + j) = *(fm + pitch * nOffY + nOffX);
			}
		}

	}
	if (nGray >= 0 && nGray <= 255) *(fmD) = nGray;

	return 0;
}


int Inspect::CopyNGImageFM(LPBYTE fm, int pitch)
{
	int i, j;
	int nOffX = _DefectData.pArea[_DefectData.Count].left;
	int nOffY = _DefectData.pArea[_DefectData.Count].top;
	int nOrgPitch = _pSystem->Pitch;
	LPBYTE fmD = _DefectData.pImage[_DefectData.Count];

	if (pitch / nOrgPitch < 1)
	{
		nOffX = nOffX * pitch / nOrgPitch - 32; if (nOffX < 0) nOffX = 0;
		nOffY = nOffY * pitch / nOrgPitch - 32; if (nOffY < 0) nOffY = 0;
	}

	/*if (g_System.m_nDefectApplyLUT)
	{
		for (i = 0; i < BAD_IMG_HEIGHT; i++)
			for (j = 0; j < BAD_IMG_WIDTH; j++)
			{
				*(fmD + BAD_IMG_WIDTH * i + j) = g_DefectLUT[*(fm + pitch * (i + nOffY) + j + nOffX)];
			}
	}
	else*/
	{
		for (i = 0; i < BAD_IMG_HEIGHT; i++)
			for (j = 0; j < BAD_IMG_WIDTH; j++)
			{
				*(fmD + BAD_IMG_WIDTH * i + j) = *(fm + pitch * (i + nOffY) + j + nOffX);
			}
	}

	return 0;
}

//0: 겹치는 것 없음
//1: 겹치는데 현재 검출한 것이 우선
//2: 겹치는데 현재 검출한 것을 버림.
int Inspect::CheckDefectOverlap(double instRngX, double instRngY)
{
	if (_DefectData.Count > _pSystem->MaxDefect ) return 2;

	int nResult = 0, i;
	int nLevelC = _DefectData.pInfo[_DefectData.Count].type / MILLION;
	int nTypeC = _DefectData.pInfo[_DefectData.Count].type % CLASSDEV;
	int nMarkC, nXC, nYC;

	int nMark;
	int nLevel, nType;
	int nDiffX, nDiffY;
	int nExistNum = -1, N;
	int nOffsetX, nOffsetY;		//8mmx8mm 영역에 있으면 겹치는 것으로 간주함. (최소 64x64)

	if (instRngX == 0.0 || instRngY == 0.0)
	{
		nOffsetX = _pSystem->OverlapX;	//Overlap Range X
		nOffsetY = _pSystem->OverlapY;	//Overlap Range Y

		if (_pParam->Cam.ScaleX > 0) nOffsetX = (int)(nOffsetX / _pParam->Cam.ScaleX);
		if (_pParam->Cam.ScaleY > 0) nOffsetY = (int)(nOffsetY / _pParam->Cam.ScaleY);

		if (nOffsetX < 64) nOffsetX = 64;	//Pixel로 따져서 64x64 보다 안쪽이면 같은 위치로 간주함.
		if (nOffsetY < 64) nOffsetY = 64;
	}
	else
	{
		nOffsetX = (int)(instRngX / _pParam->Cam.ScaleX);
		nOffsetY = (int)(instRngY / _pParam->Cam.ScaleY);
	}

	if (nLevelC < 1 || nLevelC > 10 || nTypeC < 1 || nTypeC > 9) return 2; //현재 검출한 데이타가 쓰레기라 버린다.

	nMarkC = _pMark->Marking[nTypeC - 1][nLevelC - 1];
	nXC = (int)_DefectData.pInfo[_DefectData.Count].x_pos;
	nYC = (int)_DefectData.pInfo[_DefectData.Count].y_pos;

	for (i = 0; i < _DefectData.Count; i++)
	{
		nDiffX = nXC - (int)_DefectData.pInfo[i].x_pos; if (nDiffX < 0) nDiffX *= -1;
		nDiffY = nYC - (int)_DefectData.pInfo[i].y_pos; if (nDiffY < 0) nDiffY *= -1;

		if (nDiffX < nOffsetX && nDiffY < nOffsetY)
		{
			nLevel = _DefectData.pInfo[i].type / MILLION;
			nType = _DefectData.pInfo[i].type % CLASSDEV;
			nMark = _pMark->Marking[nType - 1][nLevel - 1];

			if (nMarkC == nMark) //1. 같은 마킹이면------------------------------------------------------------
			{
				if (nLevelC == nLevel)		//A. Level이 같으면 Value 또는 Size가 큰 것으로 선택
				{
					if (nType == g_NGType._Scratch && nTypeC == g_NGType._Scratch)  //둘다 스크라치면 Value가 큰 것으로 선택
					{
						if (_DefectData.pInfo[_DefectData.Count].value > _DefectData.pInfo[i].value)
						{
							nExistNum = i;    //현재검출한 불량 선택
						}
						else
						{
							nResult = 2;		//현재불량 버림
						}
						break;
					}
					else
					{
						if ((nType == g_NGType._SpotPress && (nTypeC != g_NGType._SpotW && nTypeC != g_NGType._Cunic && nTypeC != g_NGType._SpotB)) ||
							nType == g_NGType._LScratch) //기존불량만 스크라치이면 현재불량 버림
						{
							nResult = 2;			//현재불량 버림
							break;
						}

						if (nTypeC == g_NGType._Scratch && nType == g_NGType._LScratch) //현재검출한게 스크라치이면 기존불량을 엎어침.
						{
							nExistNum = i;		//현재검출한 불량 선택
							break;
						}
						// 231103 쿠닉 확인이 필요해보임
						else if (nType == g_NGType._Scratch && nTypeC == g_NGType._SpotW && _pParam->Common.AlgorithmType == COS1)
						{
							// 밝은 불량은 휘점 불량으로 처리한다.
							if (_DefectData.pInfo[_DefectData.Count].value + _pSystem->FlatBright> 220)
							{
								nExistNum = i;    //현재검출한 불량 선택
								break;
							}
						}
						else if (nType == g_NGType._SpotB && nTypeC == g_NGType._SpotW && _pParam->Common.AlgorithmType == COS1) // 백점 있으면 백점으로 변경
						{
							nExistNum = i;    //현재검출한 불량 선택
							break;
						}
						else if ((nType == g_NGType._SpotB || nType == g_NGType._SpotW) && nTypeC == g_NGType._Cunic && _pParam->Common.AlgorithmType == COS1)
						{
							nExistNum = i;    //현재검출한 불량 선택
							break;
						}
						else if (nType == g_NGType._Group && nTypeC == g_NGType._SpotW && _pParam->Common.AlgorithmType == COS5)
						{
							nResult = 2;			//현재불량 버림
							break;
						}

						//둘다 스크라치가 아니면 Size로 구분
						if (_DefectData.pInfo[_DefectData.Count].size > _DefectData.pInfo[i].size &&
							(_DefectData.pInfo[_DefectData.Count].size - _DefectData.pInfo[i].size > 0.1))
						{
							nExistNum = i;    //현재검출한 불량 선택
						}
						else if (_DefectData.pInfo[_DefectData.Count].size == _DefectData.pInfo[i].size) //Size가 같으면 Value 비교
						{
							if (_DefectData.pInfo[_DefectData.Count].value > _DefectData.pInfo[i].value)
								nExistNum = i;
							else
								nResult = 2;		//현재불량 버림
						}
						else
							nResult = 2;		//현재불량 버림
					}
				}
				else					//B. Level이 다르면 Level낮은것 선택(강불량 선택)
				{
					if (nLevel > nLevelC)	//B-1. 기존 불량이 Level이 높으면(약불량)이면 현재것으로 대체한다.
						nExistNum = i;
					else				//B-2. 현재불량이 Level이 높으면(약불량)이면 현재것 버린다.
						nResult = 2;
				}
			}
			else	//2.  마킹이 다르면 마킹하는 것 우선-------------------------------------------------
			{
				if (nMarkC)  //현재 검출한 것이 마킹이면 기존데이타를 현재 검출한 것으로 대체해버림.
					nExistNum = i;
				else //같은 위치에 마킹이 있으면 현재 검출한 것은 버린다.
					nResult = 2;
			}
			break;
		}
	}

	if (nExistNum >= 0 && nExistNum < _pSystem->MaxDefect)
	{
		N = _DefectData.Count;

		_DefectData.pInfo[nExistNum].x_pos = _DefectData.pInfo[N].x_pos;
		_DefectData.pInfo[nExistNum].y_pos = _DefectData.pInfo[N].y_pos;
		_DefectData.pInfo[nExistNum].type =  _DefectData.pInfo[N].type;
		_DefectData.pInfo[nExistNum].size =  _DefectData.pInfo[N].size;
		_DefectData.pInfo[nExistNum].value = _DefectData.pInfo[N].value;
		_DefectData.pInfo[nExistNum].sizeX = _DefectData.pInfo[N].sizeX;
		_DefectData.pInfo[nExistNum].sizeY = _DefectData.pInfo[N].sizeY;
		for (i = 0; i < 2; i++)
			_DefectData.pInfo[nExistNum].dTemp[i] = _DefectData.pInfo[N].dTemp[i];

		//파일이름
		memcpy(_DefectData.pInfo[nExistNum].filename, _DefectData.pInfo[N].filename, MAX_BADIMAGE_FILENAME);

		//사각형--------------------------------------------------------------
		memcpy(&_DefectData.pArea[nExistNum], &_DefectData.pArea[N], sizeof(RECT));

		//Image--------------------------------------------------------------
		memcpy(_DefectData.pImage[nExistNum], _DefectData.pImage[N], BAD_IMG_WIDTH * BAD_IMG_HEIGHT);

		nResult = 1 + nExistNum * 10;
	}
	else
	{
		if (nResult == 0 && _DefectData.Count >= _pSystem->MaxDefect)
			nResult = 3;
	}

	return nResult;
}

//불량이 최대갯수보다 많을때 검사불량을 기존불량중에 약한 것으로 대체하는 함수
//현재 검출한 불량을 최우선으로 할꺼면 nIsPrior=1 아니면 0
void Inspect::CheckPriorLevel(int nIsPrior)
{
	int nLevelC = _DefectData.pInfo[_DefectData.Count].type / MILLION;
	int nTypeC = _DefectData.pInfo[_DefectData.Count].type % CLASSDEV;	//현재불량 TYPE
	int nMarkC, nXC, nYC;

	int nLevel, nType, nMark;

	int i, N, nLevelMax = 0, nExistNum = -1;

	if (nLevelC < 1 || nLevelC > 10 || nTypeC < 1 || nTypeC > 9)	return; //현재 검출한 데이타가 불량이라 버린다.
	if (_DefectData.Count < _pSystem->MaxDefect)					return; //불량갯수가 최대보다 작으면 넘어감.

	nMarkC = _pMark->Marking[nTypeC - 1][nLevelC - 1];
	nXC = (int)_DefectData.pInfo[_DefectData.Count].x_pos;
	nYC = (int)_DefectData.pInfo[_DefectData.Count].y_pos;

	if (nMarkC) //현재불량이 마킹하는 불량이면
	{
		//마킹안하는 불량중에 Level이 약한 불량 선택
		for (i = _DefectData.Count - 1; i >= 0; i--)
		{
			nLevel = _DefectData.pInfo[i].type / MILLION;
			nType = _DefectData.pInfo[i].type % CLASSDEV;
			nMark = _pMark->Marking[nType - 1][nLevel - 1];

			if (nMark == 0)
			{
				if (nLevel > nLevelMax)
				{
					nLevelMax = nLevel;
					nExistNum = i;
				}
			}
		}
		//마킹하는 불량중에 Level이 약한 불량선택
		if (nLevelMax == 0)
		{
			for (i = _DefectData.Count - 1; i >= 0; i--)
			{
				nLevel = _DefectData.pInfo[i].type / MILLION;
				nType = _DefectData.pInfo[i].type % CLASSDEV;

				if (nType == nTypeC && nLevel == nLevelC) continue;	//같은 불량 같은 Level은					 PASS
				if (nLevel < nLevelC)                   continue;	//일단 Level이 낮으면						 PASS			
				if (nIsPrior == 0 && nLevel == nLevelC)  continue;	//우선순위가 있는 불량이 아니면 같은 Level도 PASS 			

				if (nLevel > nLevelMax)
				{
					nLevelMax = nLevel;
					nExistNum = i;
				}
			}
		}
	}
	else
	{
		for (i = _DefectData.Count - 1; i >= 0; i--)
		{
			nLevel = _DefectData.pInfo[i].type / MILLION;
			nType = _DefectData.pInfo[i].type % CLASSDEV;
			nMark = _pMark->Marking[nType - 1][nLevel - 1];

			if (nMark == 0) //현재 검출한 불량이 마킹안하는 불량이므로 마킹안하는 불량중에서 고른다.
			{
				if (nType == nTypeC && nLevel == nLevelC) continue; //같은 불량 같은 Level은						 PASS
				if (nLevel < nLevelC)                   continue;	//일단 Level이 낮으면						 PASS		
				if (nIsPrior == 0 && nLevel == nLevelC)  continue;	//우선순위가 있는 불량이 아니면 같은 Level도 PASS 

				if (nLevel > nLevelMax)
				{
					nLevelMax = nLevel;
					nExistNum = i;
				}
			}
		}
	}

	if (nLevelMax < 1) return;	//아무것도 못찾았으면 PASS

	if (nExistNum >= 0 && nExistNum < _pSystem->MaxDefect)
	{
		N = _DefectData.Count;

		_DefectData.pInfo[nExistNum].x_pos = _DefectData.pInfo[N].x_pos;
		_DefectData.pInfo[nExistNum].y_pos = _DefectData.pInfo[N].y_pos;
		_DefectData.pInfo[nExistNum].type =  _DefectData.pInfo[N].type;
		_DefectData.pInfo[nExistNum].size =  _DefectData.pInfo[N].size;
		_DefectData.pInfo[nExistNum].value = _DefectData.pInfo[N].value;
		_DefectData.pInfo[nExistNum].sizeX = _DefectData.pInfo[N].sizeX;
		_DefectData.pInfo[nExistNum].sizeY = _DefectData.pInfo[N].sizeY;
		for (i = 0; i < 2; i++)
			_DefectData.pInfo[nExistNum].dTemp[i] = _DefectData.pInfo[N].dTemp[i];

		//파일이름
		memcpy(_DefectData.pInfo[nExistNum].filename, _DefectData.pInfo[N].filename, MAX_BADIMAGE_FILENAME);

		//사각형--------------------------------------------------------------
		_DefectData.pArea[nExistNum].left =   _DefectData.pArea[N].left;
		_DefectData.pArea[nExistNum].top =    _DefectData.pArea[N].top;
		_DefectData.pArea[nExistNum].right =  _DefectData.pArea[N].right;
		_DefectData.pArea[nExistNum].bottom = _DefectData.pArea[N].bottom;

		//Image--------------------------------------------------------------
		memcpy(_DefectData.pImage[nExistNum], _DefectData.pImage[N], BAD_IMG_WIDTH * BAD_IMG_HEIGHT);
	}

	return;
}


//긴것 두줄 있는 경우
int Inspect::IsOverKillForScratch(LPBYTE fm, int nX, int nY, int pitch, int nHeight, int nValue)
{
	int ret = 1; //과검 
	int i, j, k, y1, y2;
	int left, top, right, bottom, nHCount = 0;
	int nAveProj, nMaxProj, nMaxProj1, nPos;
	int nProfile[BAD_IMG_WIDTH];
	int nSumL, nSumR;

	left = nX - BAD_IMG_WIDTH / 2; if (left < 0) left = 0; if (left + BAD_IMG_WIDTH >= pitch) left = pitch - BAD_IMG_WIDTH - 1;
	right = left + BAD_IMG_WIDTH;


	top = nY - BAD_IMG_HEIGHT / 2; if (top < 0) top = 0;  if ((top + BAD_IMG_HEIGHT) >= nHeight) top = nHeight - BAD_IMG_HEIGHT - 1;
	bottom = top + BAD_IMG_HEIGHT;


	nSumL = nSumR = 0;
	for (i = top; i < bottom; i++)
	{
		nSumL += *(fm + i * pitch + left);
		nSumR += *(fm + i * pitch + right - 1);
	}
	if (nSumL / (bottom - top) > 150 || nSumR / (bottom - top) > 150) return 1;

	for (k = 0; k < 7; k++)
	{
		y1 = top + BAD_IMG_WIDTH * k / 7;
		y2 = y1 + 32;
		if (y2 >= nHeight) { y2 = nHeight; y1 = y2 - 32; }

		memset(nProfile, 0, BAD_IMG_WIDTH * sizeof(int));
		for (i = y1; i < y2; i++)
		{
			for (j = left; j < right; j++)
				nProfile[j - left] += *(fm + pitch * i + j);
		}

		nAveProj = nMaxProj = nMaxProj1 = 0;
		for (j = 0; j < BAD_IMG_WIDTH; j++)
		{
			if (nProfile[j] > nMaxProj)
			{
				nMaxProj = nProfile[j];
				nPos = j;
			}
			nAveProj += nProfile[j];
		}
		nAveProj /= BAD_IMG_WIDTH;

		if ((nMaxProj - nAveProj) * 100 / 32 > 1500)
			return 0;

		if ((nMaxProj - nAveProj) * 100 / 32 < nValue)
			continue;

		ret = 0;
		break;
	}

	return ret;
}

int Inspect::CheckRealScratch(LPBYTE fm, int nX, int nY, int pitch, int nMaxWidth, int nMaxHeight, int nMinLength)
{
	int nOverKill = 1;
	int i, j;
	int left, top, right, bottom;
	int nBlob, nY1, nY2;
	int nThres, nTmp;
	int nBlobCount = 0;
	int nXOffset = 64; //나중 32
	int nBinPitch = 40;
	int nMaxY[2] = { 0,0 }, nMaxT[2], nMaxB[2], nMaxPos[2] = { -1,-1 };
	int nProfileY[128], nProfileCount = 0;

	nBinPitch = nXOffset * 2;
	left = nX - nXOffset;
	right = nX + nXOffset;
	top = nY - 64;
	bottom = nY + 64;
	if (left < 0)				left = 0;
	if (right >= nMaxWidth)	right = nMaxWidth - 1;
	if (top < 0) { top = 0; bottom = top + 128; }
	if (bottom >= nMaxHeight) { bottom = nMaxHeight - 1; top = bottom - 128; }


	nThres = 10;
	memset(nProfileY, 0, 128 * sizeof(int));

	for (i = top + 1; i < bottom - 1; i++)
		for (j = left + 1; j < right - 1; j++)
		{
			nTmp = *(fm + pitch * (i - 1) + j - 1) + *(fm + pitch * i + j - 1) + *(fm + pitch * (i + 1) + j - 1) -
				(*(fm + pitch * (i - 1) + j + 1) + *(fm + pitch * i + j + 1) + *(fm + pitch * (i + 1) + j + 1));

			if (nTmp >= nThres) *(_pFmTemp + nBinPitch * (i - top) + (j - left)) = 255;
			else             *(_pFmTemp + nBinPitch * (i - top) + (j - left)) = 0;
		}

	BlobElongation(_pFmTemp, 1, 1, (right - left) - 2, (bottom - top) - 2, 128, 4);

	_pChain->SetChainData(1, _pFmTemp, 1, 1, 4, 1000, nBinPitch, 128);
	nBlob = _pChain->FastChain(1, 1, (right - left) - 2, (bottom - top) - 2);
	if (nBlob)
	{
		for (i = 0; i < nBlob; i++)
		{
			nY1 = _pChain->FindMinY(i);
			nY2 = _pChain->FindMaxY(i);
			if ((nY2 - nY1) > nMaxY[0])
			{
				nMaxY[0] = (nY2 - nY1);
				nMaxPos[0] = i;
				nMaxT[0] = nY1;
				nMaxB[0] = nY2;
			}
		}
		if (nMaxY[0] >= nMinLength) nOverKill = 0;
	}
	return nOverKill;
}


void Inspect::BlobElongation(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinLength)
{
	int i, j;
	int nW, nH, nX, nY;
	int nAdd;
	int nBlob, * pixel, nBoundary;
	double cx, cy, dist, distMax;
	int nPx[2], nPy[2];

	if (nMinLength < 2) nMinLength = 2;//최소 2
	_pChain->SetChainData(1, fm, 1, 1, 4, 1000, pitch, bottom);
	nBlob = _pChain->FastChain(left, top, right, bottom);

	for (i = 0; i < nBlob; i++)
	{
		nW = _pChain->FindMaxX(i) - _pChain->FindMinX(i);
		nH = _pChain->FindMaxY(i) - _pChain->FindMinY(i);
		if (nW < nMinLength && nH < nMinLength) continue;
		if (nW > nH) nAdd = nW / 2;
		else      nAdd = nH / 2;

		_pChain->Chain_Center(i, &cx, &cy);
		nBoundary = _pChain->GetChainDataNumber(i);
		pixel = _pChain->GetChainData(i);

		//중심에서 제일 먼위치 찾기
		distMax = 0;
		for (j = 0; j < nBoundary; j++)
		{
			dist = (pixel[j * 2] - cx) * (pixel[j * 2] - cx) + (pixel[j * 2 + 1] - cy) * (pixel[j * 2 + 1] - cy);
			if (dist > distMax)
			{
				distMax = dist;
				nPx[0] = pixel[j * 2];
				nPy[0] = pixel[j * 2 + 1];
			}
		}
		//(nPx[0], nPy[0])에서 제일 먼위치 찾기
		distMax = 0;
		for (j = 0; j < nBoundary; j++)
		{
			dist = (pixel[j * 2] - nPx[0]) * (pixel[j * 2] - nPx[0]) + (pixel[j * 2 + 1] - nPy[0]) * (pixel[j * 2 + 1] - nPy[0]);
			if (dist > distMax)
			{
				distMax = dist;
				nPx[1] = pixel[j * 2];
				nPy[1] = pixel[j * 2 + 1];
			}
		}
		dist = sqrt((double)(nPx[1] - nPx[0]) * (nPx[1] - nPx[0]) + (nPy[1] - nPy[0]) * (nPy[1] - nPy[0])) * 0.5;
		cx = (nPx[0] + nPx[1]) / 2;
		cy = (nPy[0] + nPy[1]) / 2;

		for (j = 1; j <= nAdd; j++)
		{
			nX = (int)(((dist + j) * nPx[0] - j * cx) / dist);
			nY = (int)(((dist + j) * nPy[0] - j * cy) / dist);
			if (nX > left && nX<right && nY>top && nY < bottom)
				*(fm + pitch * nY + nX) = 255;

			nX = (int)(((dist + j) * nPx[1] - j * cx) / dist);
			nY = (int)(((dist + j) * nPy[1] - j * cy) / dist);
			if (nX > left && nX<right && nY>top && nY < bottom)
				*(fm + pitch * nY + nX) = 255;
		}
	}
	for (i = top; i < bottom; i++)
		for (j = left; j < right; j++)
			if (*(fm + pitch * i + j) > 0) *(fm + pitch * i + j) = 255;
}

void Inspect::GetCunicValue_SameFrame(LPBYTE fm, int nX, int nY, int nMinX, int nMaxX, int pitch, int nMaxHeight, int nCunicSize, double* pCunic, int* pCunicValue)
{
	int i, j;
	int ll, tt, rr, bb;
	double dMaxCunic = 0, dCunic;
	int nMaxPosX = -1, nMaxPosY = -1;
	int nSum = 0, nAvg;
	int nMaxValue = 0;
	double  dBaseStd[2];

	//영상양끝이 일반적으로 이상하기 때문에 양끝 10Pixel은 사용하지 않는다.---
	if (nMinX < 10)	   nMinX = 10;
	if (nMaxX > pitch - 10) nMaxX = pitch - 10;
	//------------------------------------------------------------------------

	*pCunic = *pCunicValue = 0;
	if (nMaxX - nMinX - 2 < 2 * nCunicSize) return;

	ll = nX - nCunicSize;   if (ll < nMinX + 1)   ll = nMinX + 1;
	rr = ll + nCunicSize * 2; if (rr > nMaxX - 1) { rr = nMaxX - 1; ll = rr - nCunicSize * 2; }

	tt = nY - nCunicSize;   if (tt < 1) tt = 1;
	bb = tt + nCunicSize * 2; if (bb > nMaxHeight - 1) { bb = nMaxHeight - 1; tt = bb - nCunicSize * 2; }

	//평균값, 최대값 구함--------------------------------------------------
	for (i = tt; i < bb; i++)
		for (j = ll; j < rr; j++)
		{
			nSum += *(fm + pitch * i + j);
			if (*(fm + pitch * i + j) > nMaxValue) nMaxValue = *(fm + pitch * i + j);
		}
	nAvg = nSum / ((rr - ll) * (bb - tt));
	*pCunicValue = nMaxValue - nAvg;
	if (*pCunicValue > 99) *pCunicValue = 99;
	//--------------------------------------------------------------------

	tt = nY - nCunicSize / 2 - 20; if (tt < 0) tt = 0;
	bb = tt + nCunicSize;

	for (i = 0; i < 5; i++)
	{
		ll = nX - nCunicSize / 2 - 20; if (ll < nMinX) ll = nMinX;
		rr = ll + nCunicSize;

		for (j = 0; j < 5; j++)
		{
			dCunic = FinStd_WithLimitCut(fm, nAvg, ll, tt, rr, bb, pitch);

			if (dCunic > dMaxCunic)
			{
				dMaxCunic = dCunic;
				nMaxPosX = ll;
				nMaxPosY = tt;
			}

			ll += 10;
			rr += 10;
			if (rr >= nMaxX)
				break;
		}
		tt += 10;
		bb += 10;
		if (bb >= nMaxHeight)
			break;
	}
	if (nMaxPosY < 0) return;
	//최대값 찾기---------------------------------------------------
	nMaxValue = 0;
	for (i = nMaxPosY; i < nMaxPosY + nCunicSize; i++)
		for (j = nMaxPosX; j < nMaxPosX + nCunicSize; j++)
		{
			if (*(fm + pitch * i + j) > nMaxValue) nMaxValue = *(fm + pitch * i + j);
		}
	//--------------------------------------------------------------	

	if (dMaxCunic > 0)
	{
		if (nMaxPosX - nCunicSize * 2 < nMinX)			dBaseStd[0] = -101;
		else									dBaseStd[0] = FinStd_WithLimitCut(fm, nAvg, nMaxPosX - nCunicSize * 2, nMaxPosY, nMaxPosX - nCunicSize, nMaxPosY + nCunicSize, pitch);

		if (nMaxPosX + nCunicSize * 3 >= nMaxX)		dBaseStd[1] = -101;
		else									dBaseStd[1] = FinStd_WithLimitCut(fm, nAvg, nMaxPosX + nCunicSize * 2, nMaxPosY, nMaxPosX + nCunicSize * 3, nMaxPosY + nCunicSize, pitch);

		if (dBaseStd[0] > -100 && dBaseStd[1] > -100) *pCunic = dMaxCunic - (dBaseStd[0] + dBaseStd[1]) * 0.5;
		else if (dBaseStd[0] > -100)				 *pCunic = dMaxCunic - dBaseStd[0];
		else if (dBaseStd[1] > -100)				 *pCunic = dMaxCunic - dBaseStd[1];
		else									 *pCunic = 0;

		if (*pCunic < 0) *pCunic = 0;
	}
}

#define REMOVE_W_LIMIT	10 
#define REMOVE_B_LIMIT	10 

double Inspect::FinStd_WithLimitCut(LPBYTE fm, int nAveMean, int left, int top, int right, int bottom, int pitch)
{
	int i, j, nTemp;
	long nSum, nCount;
	double dSum2, dStd;
	int nArea = (right - left) * (bottom - top);

	if (left <= 0) left = 1;
	if (top <= 0)  top = 1;
	if (right >= pitch) right = pitch - 1;
	if (bottom >= _pSystem->ImageH) bottom = _pSystem->ImageH - 1;

	nArea = (right - left) * (bottom - top);

	nSum = 0;
	dSum2 = 0;
	nCount = 0;
	for (i = top; i < bottom; i++)
		for (j = left; j < right; j++)
		{
			nTemp = (*(fm + pitch * (i - 1) + j - 1) + *(fm + pitch * (i - 1) + j) + *(fm + pitch * (i - 1) + j + 1) +
					 *(fm + pitch * i + j - 1) +	   *(fm + pitch * i + j) +		 *(fm + pitch * i + j + 1) +
					 *(fm + pitch * (i + 1) + j - 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1)) / 9;

			if (nTemp > nAveMean + REMOVE_W_LIMIT) { nTemp = nAveMean; nCount++; }
			else if (nTemp < nAveMean - REMOVE_B_LIMIT) { nTemp = nAveMean; nCount++; }

			if (nTemp > nAveMean + 3)  nTemp = nAveMean + 3;

			nSum += nTemp;
			dSum2 += (nTemp * nTemp);
		}

	dStd = sqrt((nArea * dSum2 - (double)nSum * (double)nSum) / ((double)nArea * ((double)nArea - 1.)));

	return dStd;
}
