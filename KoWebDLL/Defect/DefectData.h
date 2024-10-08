#pragma once
#include "../stdafx.h"
#include "../Defines.h"

#pragma pack(push, 1)
typedef struct tagDEFECT
{
	int			type;
	float		x_pos;
	float		y_pos;
	float		size;
	float		value;

	wchar_t		filename[MAX_BADIMAGE_FILENAME];

	float       dTemp[2];							//dTemp[4]를 dTemp[2]로 줄이고, sizeX,sizeY로 구분함. dTemp[0], dTemp[1]는 마킹여부로 사용함
	float       sizeX;
	float       sizeY;

	double		roll_pos_x;			// 원단장 기준 X 위치
	double		roll_pos_y;			// 원단장 기준 Y 위치

}DEFECT, *LPDEFECT;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _stDefectData
{
	int			FrameNum;
	int			Count;
	LPRECT		pArea;
	LPBYTE		pImage[MAX_DEFECT + 1];
	LPDEFECT	pInfo;


	void Init()
	{
		for (int i = 0; i < MAX_DEFECT + 1; i++)
			pImage[i] = new BYTE[BAD_IMG_WIDTH * BAD_IMG_HEIGHT];

		pArea = new RECT[MAX_DEFECT + 1];
		pInfo = new DEFECT[MAX_DEFECT + 1];

	}

	void Release()
	{
		for (int i = 0; i < MAX_DEFECT + 1; i++)
		{
			delete[] pImage[i];
			pImage[i] = nullptr;
		}

		delete[] pArea;
		delete[] pInfo;
	}

	void Reset()
	{
		FrameNum = -1;
		Count = 0;
		memset(pArea, 0x00, sizeof(RECT) * (MAX_DEFECT + 1));
		memset(pInfo, 0x00, sizeof(DEFECT) * (MAX_DEFECT + 1));
	}

}DEFECTDATA;
#pragma pack(pop)