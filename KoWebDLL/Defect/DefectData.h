#pragma once
#include "../stdafx.h"
#include "../Defines.h"

typedef struct tagDEFECT
{
	int			type;
	float		x_pos;
	float		y_pos;
	float		size;
	float		value;

	TCHAR		filename[MAX_BADIMAGE_FILENAME];

	float       dTemp[2];							//dTemp[4]�� dTemp[2]�� ���̰�, sizeX,sizeY�� ������. dTemp[0], dTemp[1]�� ��ŷ���η� �����
	float       sizeX;
	float       sizeY;

	double		roll_pos_x;			// ������ ���� X ��ġ
	double		roll_pos_y;			// ������ ���� Y ��ġ

}DEFECT;

typedef struct _stDefectData
{
	int		FrameNum;
	int		Count;
	RECT	Area[MAX_DEFECT + 1];
	LPBYTE	pImage[MAX_DEFECT + 1];
	DEFECT	Info[MAX_DEFECT + 1];

}DEFECTDATA;