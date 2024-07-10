#include "Functions.h"

void SortData(int nCount, double *pData)
{
	int i, t;
	double dTmp = 0;

	for (i = 0; i<nCount; i++)
		for (t = i; t>0; t--)
		{
			if (pData[t]<pData[t - 1])
			{
				dTmp = pData[t];
				pData[t] = pData[t - 1];
				pData[t - 1] = dTmp;
			}
		}
}
void SortData(int nCount, long *pData)
{
	int i, t;
	long dTmp = 0;

	for (i = 0; i<nCount; i++)
		for (t = i; t>0; t--)
		{
			if (pData[t]<pData[t - 1])
			{
				dTmp = pData[t];
				pData[t] = pData[t - 1];
				pData[t - 1] = dTmp;
			}
		}
}
void SortData(int nCount, int *pData)
{
	int i, t;
	long dTmp = 0;

	for (i = 0; i<nCount; i++)
		for (t = i; t>0; t--)
		{
			if (pData[t]<pData[t - 1])
			{
				dTmp = pData[t];
				pData[t] = pData[t - 1];
				pData[t - 1] = dTmp;
			}
		}
}