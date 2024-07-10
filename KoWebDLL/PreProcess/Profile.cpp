#include <memory.h>
#include "Profile.h"
#include "../Functions.h"


Profile::Profile()
{
	m_pProfile = nullptr;
	m_pProfile100 = nullptr;
	m_pProfileH = nullptr;
	m_nProfileWidth = 0;

	m_pTmpProf = nullptr;
	m_pTmpCnt = nullptr;
}

Profile::~Profile()
{
	DeleteProfile();
}

void Profile::CreateProfile(int width)
{
	DeleteProfile();
	m_nProfileWidth = width;

	m_pProfile = new int[width];
	m_pProfile100 = new int[width];

	m_pProfileH = new int[width / 2];

	m_pTmpProf = new int[10 * width];
	m_pTmpCnt = new int[10 * width];
}

void Profile::DeleteProfile()
{
	if (m_pProfile != nullptr)
	{
		delete[] m_pProfile;
		m_pProfile = nullptr;
	}

	if (m_pProfile100 != nullptr)
	{
		delete[] m_pProfile100;
		m_pProfile100 = nullptr;
	}

	if (m_pProfileH != nullptr)
	{
		delete[] m_pProfileH;
		m_pProfileH = nullptr;
	}

	m_nProfileWidth = 0;

	if (m_pTmpProf != nullptr)
	{
		delete[] m_pTmpProf;
		m_pTmpProf = nullptr;
	}

	if (m_pTmpCnt != nullptr)
	{
		delete[] m_pTmpCnt;
		m_pTmpCnt = nullptr;
	}

}

void Profile::MakeProfileData(unsigned char* src, int width, int height, int pitch)
{
	// Profile 사이즈 확인함.
	if (m_pProfile == nullptr || m_pProfile100 == nullptr || m_nProfileWidth != width)
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
	memset(m_pTmpProf, 0x00, sizeof(int) * 10 * width);
	memset(m_pTmpCnt, 0x00, sizeof(int) * 10 * width);

	for (i = 0; i < height; i += nSkipY)
	{
		tmpIdx = i / nStep;
		tmpOffset = tmpIdx * width;
		for (j = 0; j < width; j++)
		{
			m_pTmpProf[tmpOffset + j] += *(src + pitch * i + j);
			m_pTmpCnt[tmpOffset + j]++;
		}
	}

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < width; j++)
			m_pTmpProf[i * width + j] /= m_pTmpCnt[i * width + j];
	}

	int tmpBuf[10];

	for (j = 0; j < width; j++)
	{
		for (i = 0; i < 10; i++)
			tmpBuf[i] = m_pTmpProf[i * width + j];

		SortData(10, tmpBuf);
		// 중간 데이터만 취합
		*(m_pProfile100 + j) = (tmpBuf[4] + tmpBuf[5]) / 2;
	}
	// 10개 구역으로 평균 취합한 후 중심값 취하는 방식으로 처리		- E
	////////////////////////////////////////////////////////////////////////
	
	memset(m_pProfile, 0x00, sizeof(int)*width);
	for (i = 0; i < height; i += nSkipY)
	{
		for (j = 0; j < width; j++)
		{
			nValue = *(src + pitch*i + j);
			if (nValue>*(m_pProfile100 + j) + 10)	*(m_pProfile + j) += *(m_pProfile100 + j) + 10;
			else if (nValue<*(m_pProfile100 + j) - 10)   *(m_pProfile + j) += *(m_pProfile100 + j) - 10;
			else											*(m_pProfile + j) += nValue;

			if (nValue>nMax) nMax = nValue;
			if (nValue<nMin) nMin = nValue;
		}
	}

	////////////////////////////////////////////////////////////////////////
	//LognScratchSimple을위한 추가 - S
	for (j = 0; j < width; j++)
		*(m_pProfile100 + j) = (*(m_pProfile + j) * 100) / nDevide;
	//LognScratchSimple을위한 추가 - E
	////////////////////////////////////////////////////////////////////////

	for (j = 0; j<width; j++)
	{
		*(m_pProfile + j) /= nDevide;
		nAllSum += *(m_pProfile + j);
	}

	for (j = 0; j<width / 2; j++)
		*(m_pProfileH + j) = (*(m_pProfile + 2 * j) + *(m_pProfile + 2 * j + 1)) / 2;
}
