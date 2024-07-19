#include <memory.h>
#include "Profile.h"
#include "../Functions.h"


Profile::Profile()
{
	_pProfile = nullptr;
	_pProfile100 = nullptr;
	_pProfileH = nullptr;
	_nProfileWidth = 0;

	_pTmpProf = nullptr;
	_pTmpCnt = nullptr;
}

Profile::~Profile()
{
	DeleteProfile();
}

void Profile::CreateProfile(int width)
{
	DeleteProfile();
	_nProfileWidth = width;

	_pProfile = new int[width];
	_pProfile100 = new int[width];

	_pProfileH = new int[width / 2];

	_pTmpProf = new int[10 * width];
	_pTmpCnt = new int[10 * width];
}

void Profile::DeleteProfile()
{
	if (_pProfile != nullptr)
	{
		delete[] _pProfile;
		_pProfile = nullptr;
	}

	if (_pProfile100 != nullptr)
	{
		delete[] _pProfile100;
		_pProfile100 = nullptr;
	}

	if (_pProfileH != nullptr)
	{
		delete[] _pProfileH;
		_pProfileH = nullptr;
	}

	_nProfileWidth = 0;

	if (_pTmpProf != nullptr)
	{
		delete[] _pTmpProf;
		_pTmpProf = nullptr;
	}

	if (_pTmpCnt != nullptr)
	{
		delete[] _pTmpCnt;
		_pTmpCnt = nullptr;
	}

}

void Profile::MakeProfileData(unsigned char* src, int width, int height, int pitch)
{
	// Profile 사이즈 확인함.
	if (_pProfile == nullptr || _pProfile100 == nullptr || _nProfileWidth != width)
		CreateProfile(width);

	int i, j;
	int nSkipY;
	int nDevide = 512;
	int nSum = 0;
	int nSharpSum = 0, nBrightAvg = 0, nTmp, nDiv;
	long nAllSum = 0;
	int nMax = 0, nMin = 255;
	int nValue;

	nSkipY = height / nDevide;
	if (height - nSkipY*nDevide)
	{
		if (height > 1024)
		{
			nSkipY = 2;
			nDevide = height / 2;
		}
		else
		{
			nSkipY = 1;
			nDevide = height;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// 10개 구역으로 평균 취합한 후 중심값 취하는 방식으로 처리		- S
	int nStep = (height % 10 == 0 ? height : height + 10) / 10;
	int tmpIdx, tmpOffset;
	memset(_pTmpProf, 0x00, sizeof(int) * 10 * width);
	memset(_pTmpCnt, 0x00, sizeof(int) * 10 * width);

	for (i = 0; i < height; i += nSkipY)
	{
		tmpIdx = i / nStep;
		tmpOffset = tmpIdx * width;
		for (j = 0; j < width; j++)
		{
			_pTmpProf[tmpOffset + j] += *(src + pitch * i + j);
			_pTmpCnt[tmpOffset + j]++;
		}
	}

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < width; j++)
			_pTmpProf[i * width + j] /= _pTmpCnt[i * width + j];
	}

	int tmpBuf[10];

	for (j = 0; j < width; j++)
	{
		for (i = 0; i < 10; i++)
			tmpBuf[i] = _pTmpProf[i * width + j];

		SortData(10, tmpBuf);
		// 중간 데이터만 취합
		*(_pProfile100 + j) = (tmpBuf[4] + tmpBuf[5]) / 2;
	}
	// 10개 구역으로 평균 취합한 후 중심값 취하는 방식으로 처리		- E
	////////////////////////////////////////////////////////////////////////
	
	memset(_pProfile, 0x00, sizeof(int)*width);
	for (i = 0; i < height; i += nSkipY)
	{
		for (j = 0; j < width; j++)
		{
			nValue = *(src + pitch*i + j);
			if (nValue>*(_pProfile100 + j) + 10)	*(_pProfile + j) += *(_pProfile100 + j) + 10;
			else if (nValue<*(_pProfile100 + j) - 10)   *(_pProfile + j) += *(_pProfile100 + j) - 10;
			else											*(_pProfile + j) += nValue;

			if (nValue>nMax) nMax = nValue;
			if (nValue<nMin) nMin = nValue;
		}
	}

	////////////////////////////////////////////////////////////////////////
	//LognScratchSimple을위한 추가 - S
	for (j = 0; j < width; j++)
		*(_pProfile100 + j) = (*(_pProfile + j) * 100) / nDevide;
	//LognScratchSimple을위한 추가 - E
	////////////////////////////////////////////////////////////////////////

	for (j = 0; j<width; j++)
	{
		*(_pProfile + j) /= nDevide;
		nAllSum += *(_pProfile + j);
	}

	for (j = 0; j<width / 2; j++)
		*(_pProfileH + j) = (*(_pProfile + 2 * j) + *(_pProfile + 2 * j + 1)) / 2;
}

// 검사영역으로 평균밝기 구한다.
int	Profile::CalcAvgBright(int stX, int edX, int refBright)
{
	int j, i;
	int nSum = 0;
	_AvgBright = 0;
	if (edX - stX > 10)
	{
		for (j = stX; j < edX; j++)
			nSum += *(_pProfile + j);

		_AvgBright = (double)nSum / (edX - stX);
	}
	else
	{
		_AvgBright = refBright;
	}

	return _AvgBright;
}