#include "FindEdge.h"
#include "../Param/enumParam.h"
#include "stdlib.h"
#include <memory.h>
#include "../Functions.h"

FindEdge::FindEdge()
{
	m_pProfileEdge = nullptr;
	m_nWidth = m_nHeight = 0;

	m_pTooProfile = nullptr;
	m_nImagePartH = 0;
}

FindEdge::~FindEdge()
{
	Destroy();
}

void FindEdge::Init(int width, int height)
{
	Destroy();

	m_nWidth = width;
	m_nHeight = height;

	m_pProfileEdge = new int[width];
}

void FindEdge::Destroy()
{
	if (m_pProfileEdge != nullptr)
	{
		delete[] m_pProfileEdge;
		m_pProfileEdge = nullptr;
	}

	if (m_pTooProfile != nullptr)
	{
		delete[] m_pTooProfile;
		m_pTooProfile = nullptr;
	}

	m_nWidth = m_nHeight = 0;
}

bool FindEdge::GetEdge(TempParam::INSP_EDGE_PARAM* edgeData, unsigned char* src, int width, int height, double dScaleX, int nEdgeTh, int nAlgorithm, int nOffset, int nEdgeDir, int notInspArea, bool isDualEdge)
{
	int nEdge = 0;
	int nEdgeL, nEdgeR;

	int nFoundEdge = -1;
	int nEdgeType = -1;

	bool bUseDual = false;

	if (m_nWidth != width || m_nHeight != height || m_pProfileEdge == nullptr)
		Init(width, height);

	if (nEdgeTh < 5)	// 자동  Edge라고 하고 1로 설정하는 경우 많음. 아래 주어진 값으로 Th 설정
	{
		switch (nAlgorithm)
		{
		case eParamType::eCOS1:	nEdgeTh = 30; nEdgeType = 0;  break;
		case eParamType::eCOS2: nEdgeTh = 30; nEdgeType = 10; break;
		case eParamType::eCOS3: nEdgeTh = 50; nEdgeType = 10; break;
		case eParamType::eCOS4: nEdgeTh = 120; nEdgeType = 2; break;
		case eParamType::eCOS5: nEdgeTh = 160; nEdgeType = 2; break;
		case eParamType::eCOS6: nEdgeTh = 30; nEdgeType = 1;  break;
		case eParamType::eCOS7: nEdgeTh = 30; nEdgeType = 0;  break;
		case eParamType::eCOS8: nEdgeTh = 30; nEdgeType = 0;  break;
		case eParamType::eCOS9: nEdgeTh = 40; nEdgeType = 0;  break;
		case eParamType::eCOSA: nEdgeTh = 35; nEdgeType = 3;  break;
		case eParamType::eCOSB: nEdgeTh = 30; nEdgeType = 0;  break;
		case eParamType::eCOSC: nEdgeTh = 50; nEdgeType = 2;  break;
		case eParamType::eCOSD: nEdgeTh = 50; nEdgeType = 2;  break;
		case eParamType::eCBCR: nEdgeTh = 40; nEdgeType = 2;  break;
		case eParamType::eCOSE:
			nEdgeTh = 50; 
			if(isDualEdge==false)	nEdgeType = 2;
			else { nEdgeType = 5; bUseDual = true; }
			break;
		}
	}

	switch (nEdgeType)
	{
	case 0:		// 안쪽에서 바깥쪽으로 검사(크로스) : nEdgeTh 절대값 차이나면 STOP
		nEdge = GetEdge_Sub0(src, 0, 0, width - 1, height - 1, width, (int)(3.0 / dScaleX), nEdgeTh, notInspArea);	break;
	case 1:		// 바깥쪽에서 안쪽으로 검사(최외곽 검출, 반사류) nEdgeTh 보다 밝으면 STOP
		nEdge = GetEdge_Sub1(src, 0, 0, width - 1, height - 1, width, nEdgeTh, notInspArea);	break;
	case 2:		// 안쪽에서 바깥쪽으로 검사, nEdgeTh보다 밝으면 STOP
		nEdge = GetEdge_Sub2(src, 0, 0, width - 1, height - 1, width, nEdgeTh, notInspArea);	break;
	case 3:		// 안쪽에서 바깥쪽으로 검사(투영반사, 25개 개별로 검사해서 구함)
		nEdge = GetEdge_Sub3(src, width, nEdgeTh, notInspArea);		break;
	case 4:		// 에지 차이를 판단하여 경계 확인
		nEdge = GetEdge_Sub4(src, 0, 0, width - 1, height - 1, width, nEdgeTh, nEdgeDir, notInspArea);	break;
	case 5:		// 마킹후 검출 1개 카메라 사용하는 경우 왼쪽/오른쪽 에지 검출시 사용
		GetEdge_Sub9(src, 0, 0, width - 1, height - 1, width, nEdgeTh, notInspArea, &nEdgeL, &nEdgeR);	break;
	case 10:	// 바코드 클라이언트 에지 검출
		nEdge = GetEdge_Sub10(src, 0, 0, width - 1, height - 1, width, (int)(3.0 / dScaleX), nEdgeTh, notInspArea);	break;
	default:
		nEdge = -2;
	}

	edgeData->EdgeX1 = m_nInspEdgeX1 = 0;
	edgeData->EdgeX2 = m_nInspEdgeX2 = width - 1;

	// 에지 기본 정보 업데이트

	edgeData->Dir = nEdgeDir;
	edgeData->Type = notInspArea;

	if (bUseDual == false)	// 한쪽에만 에지가 존재할 경우
	{
		if (nEdge < 0) // 데이터 이상인 경우 전체 검사
		{
			edgeData->X1 = m_nInspX1 = 0;
			edgeData->X2 = m_nInspX2 = width - 1;
		}
		else
			nEdge = GetMediumEdge(nEdge);

		if (nEdge >= 0)
		{
			if (nEdgeDir == 0)
			{
				m_nInspX1 = nEdge + nOffset;
				m_nInspX2 = width + 1;
				m_nInspEdgeX1 = nEdge;
				if (m_nInspX1 > m_nInspX2) m_nInspX1 = m_nInspX2;
			}
			else
			{
				m_nInspX1 = 0;
				m_nInspX2 = nEdge - abs(nOffset);
				m_nInspEdgeX2 = nEdge;
				if (m_nInspX2 < m_nInspX1) m_nInspX2 = m_nInspX1;
			}
			nFoundEdge = nEdge;

			edgeData->X1 = m_nInspX1;
			edgeData->X2 = m_nInspX2;
			edgeData->EdgeX1 = m_nInspEdgeX1;
			edgeData->EdgeX2 = m_nInspEdgeX2;
			edgeData->Found = nFoundEdge;
		}
	}
	else					// 양쪽에 에지가 전부 존재할 경우
	{
		if (nEdgeL < 0)
			m_nInspX1 = 0;
		else
		{
			m_nInspX1 = nEdgeL + nOffset;
			m_nInspEdgeX1 = nEdgeL;
			// 에지 기준은 왼쪽으로 처리
			m_stFoundEdge.foundEdge = nEdgeL;
		}

		if (nEdgeL < 0)
			m_nInspX2 = width - 1;
		else
		{
			m_nInspX2 = nEdgeR - nOffset;
			m_nInspEdgeX2 = nEdgeR;
		}


		edgeData->X1 = m_nInspX1;
		edgeData->X2 = m_nInspX2;
		edgeData->EdgeX1 = m_nInspEdgeX1;
		edgeData->EdgeX2 = m_nInspEdgeX2;
	}

	return true;
}

int FindEdge::GetEdge_Sub0(unsigned char* src, int left, int top, int right, int bottom, int pitch, int offset, int thUp, int notInspArea)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	long nCount, nEdge[] = { -1,-1, -1, -1, -1 };
	int nStep = 5;
	int nDir = -1;

	if (notInspArea > 0) nDir = 0;		// 왼쪽 못 씀
	else if (notInspArea < 0) nDir = 1;	// 오른쪽 못 씀
	else return -1;

	if (nDir < 0) return -1;
	m_nEdgeDir = nDir;

	for (loop = 0; loop<nStep; loop++)
	{
		nY1 = top + (bottom - top + 1)*loop / nStep;
		nY2 = top + (bottom - top + 1)*(loop + 1) / nStep;
		nOff = (nY2 - nY1) / 50;
		if (nOff<1) nOff = 1;
		nCount = 0;
		memset(m_pProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i<nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(m_pProfileEdge + j) += *(src + pitch*i + j);
		}
		if (nCount<1) return 0;

		for (j = left; j <= right; j++)
			*(m_pProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			for (j = right; j >= left + offset; j--)
			{
				if ((abs(*(m_pProfileEdge + j) - *(m_pProfileEdge + j - offset)) >= thUp) || *(m_pProfileEdge + j)>200)
				{
					nEdge[loop] = FindEdgeDetail(nDir, m_pProfileEdge, j - offset, j);
					break;
				}
			}
		}
		else
		{
			for (j = left; j <= right - offset; j++)
			{
				if (abs(*(m_pProfileEdge + j) - *(m_pProfileEdge + j + offset)) >= thUp || *(m_pProfileEdge + j)>200)
				{
					nEdge[loop] = FindEdgeDetail(nDir, m_pProfileEdge, j, j + offset);
					break;
				}
			}
		}
	}

	SortData(5, nEdge);
	if (nEdge[2] >= 0)		return nEdge[2];
	else if (nEdge[1] >= 0) return nEdge[1];
	else if (nEdge[0] >= 0) return nEdge[0];
	else  return -2;

}

int FindEdge::GetEdge_Sub1(unsigned char* src, int left, int top, int right, int bottom, int pitch, int thUp, int notInspArea)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	int nCount, nEdge = -1;
	int nStep = 5;
	int nDir = -1;

	if (notInspArea>0)		nDir = 0;   //왼쪽 못 씀 
	else if (notInspArea<0)   nDir = 1;   //오른쪽 못 씀
	else
	{
		return -1;
	}
	if (nDir<0) return -1;
	m_nEdgeDir = nDir;

	for (loop = 0; loop<nStep; loop++)
	{
		nY1 = top + (bottom - top + 1)*loop / nStep;
		nY2 = top + (bottom - top + 1)*(loop + 1) / nStep;
		nOff = (nY2 - nY2) / 50;
		if (nOff<1) nOff = 1;
		
		nCount = 0;
		memset(m_pProfileEdge, 0, (right + 1) * sizeof(int));
	
		for (i = nY1; i<nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(m_pProfileEdge + j) += *(src + pitch*i + j);
		}
		if (nCount<1) return 0;

		for (j = left; j <= right; j++)
			*(m_pProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			for (j = left; j <= right; j++)
				if (*(m_pProfileEdge + j) >= thUp)
				{
					nEdge = j;
					break;
				}
		}
		else
		{
			for (j = right; j >= left; j--)
				if (*(m_pProfileEdge + j) >= thUp)
				{
					nEdge = j;
					break;
				}
		}
	}

	return nEdge;
}

int FindEdge::GetEdge_Sub2(unsigned char* src, int left, int top, int right, int bottom, int pitch, int thUp, int notInspArea)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	int nCount, nEdge = -1;
	int nStep = 5;
	int nDir = -1;

	if (notInspArea>0)			nDir = 0;   //왼쪽 못 씀 
	else if (notInspArea<0)		nDir = 1;   //오른쪽 못 씀
	else						return -1;
	
	if (nDir<0) return -1;
	m_nEdgeDir = nDir;

	for (loop = 0; loop<nStep; loop++)
	{
		nY1 = top + (bottom - top + 1)*loop / nStep;
		nY2 = top + (bottom - top + 1)*(loop + 1) / nStep;
		nOff = (nY2 - nY2) / 50;
		if (nOff<1) nOff = 1;
		nCount = 0;
		memset(m_pProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i<nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(m_pProfileEdge + j) += *(src + pitch*i + j);
		}
		if (nCount<1) return 0;

		for (j = left; j <= right; j++)
			*(m_pProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			for (j = right; j >= left; j--)
				if (*(m_pProfileEdge + j) >= thUp)
				{
					nEdge = j;
					break;
				}
		}
		else
		{
			for (j = left; j <= right; j++)
				if (*(m_pProfileEdge + j) >= thUp)
				{
					nEdge = j;
					break;
				}
		}

		if (nEdge != -1)
			break;
	}

	return nEdge;
}

int FindEdge::GetEdge_Sub3(unsigned char* src, int pitch, int nThreshold, int notInspArea)
{
	int nFoundEdge = -1;
	int nEdgeData;
	int i, j, k;
	int nY1, nY2, nDiff;
	int nNotFound = 0, nFoundCount;
	long nEdge[50];
	int nDir;

	if (notInspArea>0)		nDir = 0;   //왼쪽 못 씀 
	else if (notInspArea<0) nDir = 1;   //오른쪽 못 씀
	else					return -1;
	
	m_nEdgeDir = nDir;

	nThreshold *= 10;

	if (m_pTooProfile == nullptr)
		m_pTooProfile = new int[m_nWidth];

	for (k = 0; k<m_nImagePart; k++)
	{
		memset(m_pTooProfile, 0, m_nWidth * sizeof(int));
		nY1 = m_nImagePartH*k + m_nImagePartH / 2 - 10;
		nY2 = nY1 + 20;
		for (i = nY1; i<nY2; i += 2)
			for (j = 0; j<m_nWidth; j++)
				*(m_pTooProfile + j) += *(src + pitch*i + j);

		nEdgeData = -1;
		if (nDir == 0) //<----이방향 Scan
		{
			for (j = m_nWidth - 10; j >= 20; j--)
			{
				nDiff = *(m_pTooProfile + j - 10) - *(m_pTooProfile + j);
				if (nDiff<0) nDiff *= -1;
				if (nDiff>nThreshold || *(m_pTooProfile + j)<300)
				{
					nEdgeData = j - 5;
					break;
				}
			}
		}
		else	//--->이 방향 Scan
		{
			for (j = 10; j<m_nWidth - 20; j++)
			{
				nDiff = *(m_pTooProfile + j + 10) - *(m_pTooProfile + j);
				if (nDiff<0) nDiff *= -1;
				if (nDiff>nThreshold || *(m_pTooProfile + j)<300)
				{
					nEdgeData = j + 5;
					break;
				}
			}
		}
		if (nEdgeData<0) nNotFound++;
		else			 nEdge[k] = nEdgeData;
	}
	nFoundCount = m_nImagePart - nNotFound;
	if (nFoundCount<m_nImagePart / 2) return nFoundEdge;

	SortData(nFoundCount, nEdge);
	nFoundEdge = nEdge[nFoundCount / 2];

	return nFoundEdge;
}

// 바코드 검사 확인용
int FindEdge::GetEdge_Sub4(unsigned char* src, int left, int top, int right, int bottom, int pitch, int thUp, int edgeOffset, int notInspArea)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	int nCount, nEdge = -1;
	int nStep = 5;
	int nDir = -1;

	if (notInspArea > 0)		nDir = 0;   //왼쪽 못 씀 
	else if (notInspArea < 0)   nDir = 1;   //오른쪽 못 씀
	else
	{
		if (edgeOffset > 0)			nDir = 0; //왼쪽 못 씀
		else if (edgeOffset < 0)    nDir = 1; //오른쪽 못 씀
	}
	if (nDir < 0) return -1;

	m_nEdgeDir = nDir;

	for (loop = 0; loop < nStep; loop++)
	{
		nY1 = top + (bottom - top + 1) * loop / nStep;
		nY2 = top + (bottom - top + 1) * (loop + 1) / nStep;
		nOff = (nY2 - nY2) / 50;
		if (nOff < 1) nOff = 1;
		nCount = 0;
		memset(m_pProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i < nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(m_pProfileEdge + j) += *(src + pitch * i + j);
		}
		if (nCount < 1) return 0;

		for (j = left; j <= right; j++)
			*(m_pProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			for (j = right - 2; j >= left - 2; j--)
				if (abs(*(m_pProfileEdge + j + 2) - *(m_pProfileEdge + j - 2)) >= thUp)
				{
					nEdge = j;
					break;
				}

			if (nEdge != -1)
			{
				int val1, val2;
				val1 = *(m_pProfileEdge + j + 2);
				val2 = *(m_pProfileEdge + j - 2);
				int eth = val1 + (val2 - val1) / 3;
				for (int ii = nEdge + 2; ii >= nEdge - 2; ii--)
				{
					if (*(m_pProfileEdge + ii) > eth)
					{
						nEdge = ii;
						break;
					}
				}
			}
		}
		else
		{
			for (j = left + 2; j < right - 2; j++)
				if (abs(*(m_pProfileEdge + j + 2) - *(m_pProfileEdge + j - 2)) >= thUp)
				{
					nEdge = j;
					break;
				}

			if (nEdge != -1)
			{
				int val1, val2;
				val1 = *(m_pProfileEdge + j - 2);
				val2 = *(m_pProfileEdge + j + 2);
				int eth = val1 + (val2 - val1) / 3;
				for (int ii = nEdge - 2; ii <= nEdge + 2; ii++)
				{
					if (*(m_pProfileEdge + ii) > eth)
					{
						nEdge = ii;
						break;
					}
				}
			}
		}

		if (nEdge != -1)
			break;
	}
	return nEdge;
}

void FindEdge::GetEdge_Sub9(unsigned char* src, int left, int top, int right, int bottom, int pitch, int thUp, int notInspArea, int* pEdgeL, int* pEdgeR)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	int nCount, nEdgeL = -1, nEdgeR = -1;
	int nStep = 5;
	int nCenter = (right + left) / 2;
	*pEdgeL = *pEdgeR = -1;

	if (notInspArea == 0) return;

	for (loop = 0; loop < nStep; loop++)
	{
		nY1 = top + (bottom - top + 1) * loop / nStep;
		nY2 = top + (bottom - top + 1) * (loop + 1) / nStep;
		nOff = (nY2 - nY2) / 50;
		if (nOff < 1) nOff = 1;
		nCount = 0;
		memset(m_pProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i < nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(m_pProfileEdge + j) += *(src + pitch * i + j);
		}
		if (nCount < 1) return;

		for (j = left; j <= right; j++)
			*(m_pProfileEdge + j) /= nCount;

		// 1. 중심 센터 기즌으로 왼쪽 탐색
		for (j = nCenter; j >= left - 9; j--)
			if (abs(*(m_pProfileEdge + j) - *(m_pProfileEdge + j - 9)) >= thUp)
			{
				nEdgeL = j;
				nEdgeL = FindEdgeDetail(0, m_pProfileEdge, j - 9, j);
				break;
			}

		// 2. 중심 센터 기준으로 오른쪽 탐색
		for (j = nCenter; j < right - 9; j++)
			if (abs(*(m_pProfileEdge + j) - *(m_pProfileEdge + j + 9)) >= thUp)
			{
				nEdgeR = j;
				nEdgeR = FindEdgeDetail(1, m_pProfileEdge, j, j + 9);
				break;
			}

		if (nEdgeL != -1)	*pEdgeL = nEdgeL;
		if (nEdgeR != -1)	*pEdgeR = nEdgeR;
		if (*pEdgeL != -1 && *pEdgeR != -1)
			break;
	}
}

int FindEdge::GetEdge_Sub10(unsigned char* src, int left, int top, int right, int bottom, int pitch, int offset, int thUp, int notInspArea)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	long nCount, nEdge[5] = { -1,-1,-1,-1,-1 };
	int nStep = 5;
	int nDir = -1;

	if (notInspArea>0)		nDir = 0;   //왼쪽 못 씀 
	else if (notInspArea<0) nDir = 1;   //오른쪽 못 씀
	else					return -1;
	
	if (nDir<0) return -1;
	
	m_nEdgeDir = nDir;

	for (loop = 0; loop<nStep; loop++)
	{
		nY1 = top + (bottom - top + 1)*loop / nStep;
		nY2 = top + (bottom - top + 1)*(loop + 1) / nStep;
		nOff = (nY2 - nY1) / 50;
		if (nOff<1) nOff = 1;
		nCount = 0;
		memset(m_pProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i<nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(m_pProfileEdge + j) += *(src + pitch*i + j);
		}
		if (nCount<1) return 0;

		for (j = left; j <= right; j++)
			*(m_pProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			for (j = right; j >= left + offset; j--)
			{
				if ((abs(*(m_pProfileEdge + j) - *(m_pProfileEdge + j - offset)) >= thUp))
				{
					nEdge[loop] = FindEdgeDetail(nDir, m_pProfileEdge, j - offset, j);
					break;
				}
			}
		}
		else
		{
			for (j = left; j <= right - offset; j++)
			{
				if (abs(*(m_pProfileEdge + j) - *(m_pProfileEdge + j + offset)) >= thUp)
				{
					nEdge[loop] = FindEdgeDetail(nDir, m_pProfileEdge, j, j + offset);
					break;
				}
			}
		}
	}

	SortData(5, nEdge);
	if (nEdge[2] >= 0)		return nEdge[2];
	else if (nEdge[1] >= 0) return nEdge[1];
	else if (nEdge[0] >= 0) return nEdge[0];
	else					return -2;
}

int FindEdge::FindEdgeDetail(int nDir, int *pProfile, int N1, int N2)
{
	int i, nFlag = 0;;
	int nMax = 0, nMin = 255, nThres;

	for (i = N1; i <= N2; i++)
	{
		if (*(pProfile + i)>nMax) nMax = *(pProfile + i);
		if (*(pProfile + i)<nMin) nMin = *(pProfile + i);
	}

	nThres = (nMax + nMin) / 2;

	if (nDir == 0)
	{
		if (*(pProfile + N1)<*(pProfile + N2))
		{
			for (i = N2; i >= N1; i--)
				if (*(pProfile + i) <= nThres)	return i;
		}
		else
		{
			for (i = N2; i >= N1; i--)
				if (*(pProfile + i) >= nThres)	return i;
		}
	}
	else
	{
		if (*(pProfile + N1)<*(pProfile + N2))
		{
			for (i = N1; i <= N2; i++)
				if (*(pProfile + i) >= nThres)	return i;
		}
		else
		{
			for (i = N1; i <= N2; i++)
				if (*(pProfile + i) <= nThres)	return i;
		}
	}

	return 0;
}

int FindEdge::GetMediumEdge(int nEdge)
{
	int nRet;
	int i, nCount = 0, nEdgeArray[MAX_EDGE_COUNT];

	if (m_stFoundEdge.count < MAX_EDGE_COUNT)
	{
		m_stFoundEdge.arrEdge[m_stFoundEdge.count++] = nEdge;
	}
	else
	{
		for (i = 0; i < MAX_EDGE_COUNT - 1; i++)
			m_stFoundEdge.arrEdge[i] = m_stFoundEdge.arrEdge[i + 1];
		m_stFoundEdge.arrEdge[MAX_EDGE_COUNT - 1] = nEdge;
	}

	for (i = 0; i < m_stFoundEdge.count; i++)
	{
		if (m_stFoundEdge.arrEdge[i] > 0) nEdgeArray[nCount++] = m_stFoundEdge.arrEdge[i];
	}

	if (nCount > 0)		nRet = GetMediumEdgeSub(nCount, nEdgeArray);
	else				nRet = -1;

	return nRet;
}

int FindEdge::GetMediumEdgeSub(int count, int *pValue)
{
	int i, j;
	int maxVal;
	int num;
	int storeVal;
	int arrVal[MAX_EDGE_COUNT];

	for (i = 0; i < count; i++)
		arrVal[i] = pValue[i];

	for (i = 0; i < count / 2; i++)
	{
		num = i;
		maxVal = storeVal = arrVal[i];

		for(j=i+1; j<count; j++)
			if (arrVal[j] > maxVal) { maxVal = arrVal[j]; num = j; }

		arrVal[i] = arrVal[num];
		arrVal[num] = storeVal;
	}

	if (count < 2)	return arrVal[0];
	else			return arrVal[count / 2 - 1];
}

