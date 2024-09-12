#include "PyramidImage.h"
#include "math.h"
#include <ppl.h>
#include "../stdafx.h"

#include "atlimage.h"

#ifdef USE_OPENCV
#include "opencv2/opencv.hpp"
#endif

PyramidImage::PyramidImage(int maxLv)
{
	if (maxLv > COMP_LEVEL)
		maxLv = COMP_LEVEL;

	_MaxLv = maxLv;

	for (int i = 0; i < COMP_LEVEL; i++)
	{
		_Image[i] = nullptr;
		_Width[i] = 0;
		_Height[i] = 0;
	}

	_RefW = 0;
	_RefH = 0;

	_IsFinish = false;
}

PyramidImage::~PyramidImage()
{
	DeleteBuf();
}

void PyramidImage::SetImageInfo(int width, int height)
{
	_RefW = width;
	_RefH = height;

	CreateBuf();
}

bool PyramidImage::MakeImage(unsigned char* buf, int width, int height)
{
	if (_RefW == 0 || _RefH == 0) return false;

	unsigned char *fmD, *fmS;
	int pitchD, pitchS;
	int nW;

	// 대용량 이미지의 경우 데이터 쓰기 속도로 인하여 원본 _Image[0]은 사용하지 않고 데이터를 처리한다. 
	//memcpy(_Image[0], buf, sizeof(unsigned char) * width * height);
	
#ifdef USE_OPENCV
	cv::Mat matFlat = cv::Mat(height, width, CV_8UC1);
	cv::Mat matPyr;

	memcpy(matFlat.data, buf, sizeof(unsigned char) * height * width);

	for (int i = 1; i < 5; i++)
	{
		fmD = _Image[i];
		cv::pyrDown(matFlat, matPyr, cv::Size(_Width[i], _Height[i]));

		memcpy(fmD, matPyr.data, sizeof(unsigned char) * _Width[i] * _Height[i]);
		matFlat.release();
		matFlat = matPyr.clone();
	}
	matFlat.release();
#else

	for (int k = 1; k < _MaxLv; k++)
	{
		clock_t time3 = clock();
		if (k == 1)		fmS = buf;
		else			fmS = _Image[k - 1];
		pitchS = _Width[k-1];
		fmD = _Image[k];
		pitchD = _Width[k];
		nW = _Width[k];

		/*#pragma omp parallel for
		for (int i = 0; i < _Height[k]; i++)
		{
			int nI;
			nI = i * 2;
			
			#pragma omp simd
			for (int j = 0; j < nW; j++)
				*(fmD + pitchD * i + j) = (*(fmS + pitchS * nI + j * 2) + *(fmS + pitchS * nI + j * 2 + 1) + *(fmS + pitchS * (nI + 1) + j * 2) + *(fmS + pitchS * (nI + 1) + j * 2 + 1)) / 4;
		};*/


		Concurrency::parallel_for(0, _Height[k], [&](int i) 
		{
			int nI, nJ, j;
			nI = i * 2;
			unsigned char* pFmD = fmD + pitchD * i;
			unsigned char* pFmS = fmS + pitchS * nI;
			for (j = 0; j < nW; j++)
			{
				nJ = j * 2;
				*(pFmD + j) = (*(pFmS + nJ) + *(pFmS + nJ + 1) + *(pFmS + pitchS + nJ) + *(pFmS + pitchS + nJ + 1) + 2) / 4;
			}
		});

		/*for (int i = 0; i < _Height[k]; i++)
		{
			int nI, nJ, j;
			nI = i * 2;
			for (j = 0; j < nW; j++)
			{
				nJ = j * 2;
				*(fmD + pitchD * i + j) = (*(fmS + pitchS * nI + nJ) + *(fmS + pitchS * nI + nJ + 1) + *(fmS + pitchS * (nI + 1) + nJ) + *(fmS + pitchS * (nI + 1) + nJ + 1)) / 4;
			}
		};*/
	}
#endif
	_IsFinish = true;

	return true;
}

void PyramidImage::DeleteBuf()
{
	for (int i = 0; i < COMP_LEVEL; i++)
	{
		if (_Image[i] != nullptr)
		{
			delete[] _Image[i];
			_Image[i] = nullptr;

			_Width[i] = 0;
			_Height[i] = 0;
		}
	}

	_IsFinish = false;
}

void PyramidImage::CreateBuf()
{
	DeleteBuf();

	// 이미지 설정이 되어있지 않으면 생성이 불가
	if (_RefW == 0 || _RefH == 0)
		return;

	_Width[0] = _RefW;
	_Height[0] = _RefH;

	// 0 번은 생성 안 함. 
	// 굳이 필요하지 않음. 
	_Image[0] = nullptr;//new unsigned char[_Width[0] * _Height[0]];

	for (int i = 1; i < _MaxLv; i++)
	{
		_Width[i] = _Width[i-1]/2;
		_Height[i] = _Height[i-1]/2;

		_Image[i] = new unsigned char[_Width[i] * _Height[i]];
	}
}