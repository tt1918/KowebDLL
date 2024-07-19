
#include "FlatImage.h"
#include "../Defines.h"
#include <ppl.h>

FlatImage::FlatImage()
{
	_pArea = new int[MAX_CROSS_WIDTH];

	_flatImg = nullptr;
}

FlatImage::~FlatImage()
{
	_projFactor.Clear();

	if (_pArea != nullptr)
	{
		delete[] _pArea;
		_pArea = nullptr;
	}

	Clear();
}

void FlatImage::Init(int width, int height)
{
	_width = width;
	_height = height;
}

void FlatImage::Clear()
{
	if (_flatImg != nullptr)
	{
		delete[] _flatImg;
		_flatImg = nullptr;
	}
	
	_width = _height = 0;
}

void FlatImage::MakeFlatLineScan(unsigned char* src, unsigned char* dst, int* profile, int flatBrt, int left, int width, int height, int pitch, int avg, int inspX1, int inspX2)
{
	int i, j;
	int right = left + width;
	int factor;
	int* projFactor;

	// Initialize Profile Data 
	_projFactor.Init(width);
	projFactor = _projFactor.Val;

	if (right <= inspX1 || left >= inspX2)
	{
		// 완료되면 종료한다. 
		for (i = 0; i < height; i++)
			memcpy(dst + pitch * i + left, src + pitch * i + left, sizeof(unsigned char) * (right - left));
		return ;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// Flat Factor 만들기 - S
	for (j = left; j < right; j++)
	{
		if (profile[j] > 0)
		{
			factor = 1000 * avg / profile[j];
			if (factor > 1000) factor = 1000 + (int)((double)(factor - 1000) * 0.75);

			if (factor > 1500)		projFactor[j - left] = 1500;
			else if (factor < 800)	projFactor[j - left] = 800;
			else					projFactor[j - left] = factor;
		}
		else
		{
			projFactor[j - left] = 1000;
		}
	}
	// Flat Factor 만들기 - E
	/////////////////////////////////////////////////////////////////////////////////////////

	if (left<inspX1 && right > inspX1)
	{
		for (i = 0; i < height; i++)
			memcpy(dst + pitch * i + left, src + pitch * i + left, sizeof(unsigned char) * (inspX1 - left));
	}
	else if (left<inspX2 && right > inspX2)
	{
		for (i = 0; i < height; i++)
			memcpy(dst + pitch * i + inspX2, src + pitch * i + inspX2, sizeof(unsigned char) * (right - inspX2));
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// Flat 수행	- S
	Concurrency::parallel_for(0, height, [&](int ii) {
		int jj, tmp;
		unsigned char* srcT, * dstT;
		int* profileTmp, * projFactorTmp;

		srcT = src + pitch * ii + left;
		dstT = dst + pitch * ii + left;

		profileTmp = profile + left;
		projFactorTmp = projFactor;
		for (jj = left; jj < right; jj++)
		{
			tmp = flatBrt + ((*(srcT++) - *(profileTmp++)) * *(projFactorTmp++) + 500) / 1000;
			if (tmp < 0)				*(dstT++) = 0;
			else if (tmp > 255)			*(dstT++) = 255;
			else						*(dstT++) = tmp;
		}
	});
	// Flat 수행	- E
	/////////////////////////////////////////////////////////////////////////////////////////
}

void FlatImage::MakeFlatArea320(unsigned char* src, unsigned char* dst, int nBaseValue, int left, int width, int height, int pitch)
{
	int right = left + width;
	int i, j;
	int nTemp = 0;
	int nCount1 = 0;
	int nImageProjection[MAX_CROSS_WIDTH];
	int nProfileRatio[MAX_CROSS_WIDTH];
	int nSum, nCount, nGray, nTmp;
	int nAVG = 0, nMaxProfile = 0, nMaxPos, nPosL, nPosR;;
	int m, k = 0;
	int nHeightH = height / 8;

	if (nHeightH <= 0) return;

	memset(nImageProjection, 0, pitch * sizeof(int));

	for (m = 0; m < 8; m++)
	{
		//한번 촬상영역의 전체 평균밝기를 구한다 -------------------------------------------------------
		/*for (j = left; j < right; j++) 
			nImageProjection[j] = m_pArea[j] = 0;*/
		memset(nImageProjection, 0x00, sizeof(int)*MAX_CROSS_WIDTH);
		memset(_pArea, 0x00, sizeof(int)*MAX_CROSS_WIDTH);

		for (i = (m * nHeightH); i < ((m + 1) * nHeightH); i += 4)
		{
			for (j = left; j < right; j++)
				nImageProjection[j] += *(src + i * pitch + j);
		}

		for (j = left; j < right; j++) 
			nImageProjection[j] /= (nHeightH / 4);

		nCount = nSum = 0;
		for (j = left; j < right; j++)
			_pArea[j] = nImageProjection[j] + 15;
		
		//------------------------------------------------------------------------------------------------

		//전체 평균보다 15이상 튄 값은 CUT하는 Projection을 구한다.----------------------------------------
		for (j = left; j < right; j++) 
			nImageProjection[j] = 0;

		for (i = (m * nHeightH); i < ((m + 1) * nHeightH); i++)
		{
			for (j = left; j < right; j++)
			{
				nTemp = *(src + i * pitch + j);
				if (nTemp > _pArea[j]) nTemp = _pArea[j];
				nImageProjection[j] += nTemp;
			}
		}

		for (j = left; j < right; j++)
			nImageProjection[j] /= nHeightH;
		//--------------------------------------------------------------------------------------------------
		nMaxPos = pitch / 2;

		if (width != pitch)
		{
			for (j = left; j < right; j++)
			{
				if (nImageProjection[j] > nMaxProfile)
				{
					nMaxProfile = nImageProjection[j];
					nMaxPos = j;
				}
			}
		}
		nPosL = nMaxPos - 50; if (nPosL < left + 50) nPosL = left + 50;   //가장 바깥쪽 +/- 50 Pixel은 사용안함.
		nPosR = nPosL + 100;  if (nPosR > right - 50) nPosR = right - 50;

		for (j = nPosL; j < nPosR; j++) nAVG += nImageProjection[j];
		if ((nPosR - nPosL) > 0)	nAVG /= (nPosR - nPosL);
		else						nAVG = nBaseValue;

		for (j = left; j < right; j++)
		{
			if (nImageProjection[j] > 0)
			{
				nTmp = 1000 * nAVG / nImageProjection[j];
				if (nTmp > 1000)	nProfileRatio[j] = 1000 + (int)((double)(nTmp - 1000) * 0.6); //60%만 증폭한다.
				else				nProfileRatio[j] = 1000;
			}
			else	
				nProfileRatio[j] = 1000;

			if (nProfileRatio[j] > 1500)		nProfileRatio[j] = 1500;
			else if (nProfileRatio[j] < 1000)	nProfileRatio[j] = 1000;
		}

		for (i = (m * nHeightH); i < (m + 1) * nHeightH; i++)
		{
			for (j = left; j < right; j++)
			{
				nGray = *(src + pitch * i + j);

				nTmp = nBaseValue + ((nGray - nImageProjection[j]) * nProfileRatio[j] + 500) / 1000;

				if (nTmp < 0)			*(dst + pitch * i + j) = 0;
				else if (nTmp > 255)	*(dst + pitch * i + j) = 255;
				else					*(dst + pitch * i + j) = nTmp;
			}
		}
	}
}