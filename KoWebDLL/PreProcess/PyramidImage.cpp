#include "PyramidImage.h"
#include "math.h"
#include <ppl.h>

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

	memcpy(_Image[0], buf, sizeof(unsigned char) * width * height);

	for (int k = 1; k < _MaxLv; k++)
	{
		fmS = _Image[k - 1];
		pitchS = _Width[k];
		fmD = _Image[k];
		pitchD = _Width[k];
		nW = _Width[k];

		Concurrency::parallel_for(0, _Height[k], [&](int i) {
			int nI, nJ, j;
			nI = i * 2;
			for (j = 0; j < nW; j++)
			{
				nJ = j * 2;
				*(fmD + pitchD * i + j) = (*(fmS + pitchS * nI + nJ) + *(fmS + pitchS * nI + nJ + 1) + *(fmS + pitchS * (nI + 1) + nJ) + *(fmS + pitchS * (nI + 1) + nJ + 1) + 2) / 4;
			}
		});
	}
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
}

void PyramidImage::CreateBuf()
{
	DeleteBuf();

	// 이미지 설정이 되어있지 않으면 생성이 불가
	if (_RefW == 0 || _RefH == 0)
		return;

	_Width[0] = _RefW;
	_Height[0] = _RefH;

	_Image[0] = new unsigned char(_Width[0] * _Height[0]);

	for (int i = 1; i < _MaxLv; i++)
	{
		_Width[i] = _Width[i-1]/2;
		_Height[i] = _Height[i-1]/2;

		_Image[i] = new unsigned char(_Width[i] * _Height[i]);
	}
}