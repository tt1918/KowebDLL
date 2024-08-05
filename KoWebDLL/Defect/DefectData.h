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

	wchar_t		filename[MAX_BADIMAGE_FILENAME];

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


	void Init()
	{
		for (int i = 0; i < MAX_DEFECT + 1; i++)
		{
			pImage[i] = new BYTE[BAD_IMG_WIDTH * BAD_IMG_HEIGHT];
		}
	}

	void Release()
	{
		for (int i = 0; i < MAX_DEFECT + 1; i++)
		{
			delete[] pImage[i];
		}
	}

	void Reset()
	{
		FrameNum = -1;
		Count = 0;
		memset(Area, 0x00, sizeof(RECT) * (MAX_DEFECT + 1));
		memset(Info, 0x00, sizeof(DEFECT) * (MAX_DEFECT + 1));
	}

}DEFECTDATA;