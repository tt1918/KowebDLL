#pragma once
#include "../Defines.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������� �޴� �˻� Parameter - s

#define MAX_CNADIDATE_AREA	1280
namespace InspParam
{
	typedef struct _stCommonParam
	{
		bool	useRandomCycle;
		bool	useAi;
		bool	useFlatImage;

		int		TrgGrayDiff;	// �ڵ� ���� ��� ���� ����
		int		MakePyramid;	// 

		int		BrightMax;
		int		BrightMin;

		int		PeriodLevel;	// �ֱ� �ҷ��� 1�� ����
		int		SizeMethod;		// ������ ���� ���

		int		NoInspection;	// No �˻�
		int		EdgeTh;			// ���� ���� TH
		int		EdgeOffset;		// ���� ���� �� ���� �ɼ�

		int		AlgorithmType;	// �˰��� Ÿ��

		int		AttachPxl;		// �̿��ϴ� Blob���̴� ��

		int		TapeDist[3];	// 0: ���԰Ÿ�, 1: �������԰Ÿ�, 2: ��ȣ ���� �Ÿ�
		int		TapeSkipDist;	// ���� �ν� �� ��ŵ �Ÿ�
		int		TapeToOpticDist;	// ���� ���� <-> ���а� ������ �Ÿ�

		int		BrightCompensation; // ��� ����
	}COMMON_PARAM;

	typedef struct _stHoleSkipParam
	{
		bool	UseCheck;
		int		Th;
		double  Period;
		double	Size;
		int		RemoveTh;
		double	ErrRatio;
		float	CompactMin;
		float	CompactMax;
		float	Maxdiff;
	}HOLE_SKIP_PARAM;

	typedef struct _stCamParam
	{
		int Exposure;
		double SttPosX;
		double SttPosY;
		double FrameLeng;
		double ScaleX;
		double ScaleY;

		double AngleFactorY;
	}CAM_PARAM;

	typedef struct _stBasicInspParam
	{
		int TH;
		double Size;
	}BASIC_INSP_PARAM;

	typedef struct _stSpotParam
	{
		int		AreaTh;

		int		LvTH[MAX_LEVEL];
		double	LvSize[MAX_LEVEL];
	}SPOT_PARAM;

	typedef struct _stBiningParam
	{
		int		ID;				// Bining Mode ������ �ε���
		bool	Updated;		// ���̴� ��� ������ ������Ʈ �߻� �� ó��
		int		CvtByParam;		// �Ķ���Ϳ��� ������ �о���� �� ���� ó��

		int		OldMode;		// ���� ���̴� ��� �ε���
		bool	IsConvert;		// ���̴� ��� ���� ���� ���

		int		InitStableTime;	// ī�޶� ���� ����ȭ �ð�
	}BINING_PARAM;

	typedef struct _stPinhole
	{
		int TH;						// ��Ȧ TH
		int ShadowOffset;			// �׸��� �ν� �Ÿ�
		int ShadowTH;				// Pinhole���� �׸��� Size ���� �� ����ϴ� TH
		double ShadowSkipSize;		// �׸��� Skip Size( �׸��� ����� �̰����� ũ�� Skip �Ѵ�)
		double ShadowNotSkipSize;	// �׸��� Skip ���� Size(���� AreaTH�� ���� Size�� �� ������ ũ�� �׸��� ������� Skip���� �ʴ´�.)

	}PINHOLE_PARAM;

	typedef struct _stGroupItem
	{
		int		Area;	// ���� ����
		int		Count;	//  ����
		int		TH;		// ���� �ν� ��� TH;
		double	Size;	// ���� ������
	}GROUP_ITEM;

	// �決�� or Cross ����
	typedef struct _stGroupParam
	{
		int		TH;				// ���� Area TH
		int		SizeMethod;		// ���� ������ ��� ��� 0:����, 1: �밢��
		int		CvtLevel;		// �ҷ��� ������ ����
		
		GROUP_ITEM Base;
		GROUP_ITEM Level[MAX_LEVEL];

	}GROUP_PARAM;

	// ��� ���� �Ķ����
	typedef struct _stCunicParam
	{
		int		InspArea;		// ��а˻� Area
		double  CandiVal;		// ��� �ĺ� �ּҰ�
		double  Std[MAX_LEVEL];		// �������� ���  STD
		double  Std1[MAX_LEVEL];		// ��� Value1�� ¦�� �̷�
		double	Value1[MAX_LEVEL];		// ��� Std1�� ¦�� �̷�
	}CUNIC_PARAM;

	// SC �Ķ����
	typedef struct _stSC
	{
		int		Offset;					// ��ũ��ġ �˻� ��
		int		ScVal[MAX_LEVEL];		// ��ũ��ġ Value
	}SC_PARAM;

	// Long SC �Ķ����
	typedef struct _stLongSC
	{
		int		FrameSize;				// Long Scratch �˻��� frame Size
		int		MinFrameCnt;			// �ҷ� �߻� �� �ּ� �ҷ� Frame ������

		int		LongScVal[MAX_LEVEL];	// Long ��ũ��ġ Value
	}LONG_SC_PARAM;

	typedef struct _stPress
	{
		bool	IsInsp;				// �˻� ��� ����

		int		UpTh;				// ���� ���� �ּ� ���� TH
		int		DnTh;				// ���� ���� �ּ� ���� TH

		int		AreaUpTH;			// ���� ���� TH
		int		AreaDnTH;			// ���� ���� TH

		// Level Value
		int		Value[MAX_LEVEL];	// ��� ��
		double	Size[MAX_LEVEL];	// �ּ� ������
	}PRESS_PARAM;

	typedef struct _stKipo
	{
		int		Value[MAX_LEVEL];		// ���� �̹� Value
		double	Size[MAX_LEVEL];		// ���� �̹� Size
		int		MinVal;					// ���� �̹� �ּ� Value
		double	BrightDiff;				// ���� �̹� �ּ� �����
	}KIPO_PARAM;


	typedef struct _stBCR
	{
		////////////////////////////////////////////////////////////////
		// BCR - S

		// ���� ���� ����
		int		SearchWidth;		// 10

									// BCR Size
		double	Width;				// 12
		double	Height;				// 13
		double	DiffW;				// 14
		double	DiffH;				// 15
									//  
		int		DotTh;				// 18	

									// BCR ���
		bool    ObjW;				// 20

									// ���� ���� ���� ����
		bool	UseManualArea;		// 21
		int		AreaL;				// 22
		int		AreaR;				// 23

									// Ž�� �� ���� ��� ���� �˻� ó��
		bool	UseMatSize;			// 24
		int		WarningM;			// 25
		int		ErrorM;				// 26

		bool	DoSaveImage;		// 27

				// =======================================================
				// ���� Ȥ�� UI���� �޾ƿ��� �κ�
		int		MarkingDefect;

		// Bcr ��ġ�� ��ŷ ���� ����
		int		BCDPos;

		// ���� Ÿ��
		int		CsvType;
		bool	useES;
		bool	useTG;
		bool	useETC;
		// =======================================================

		// �θ��� ��Ʈ üũ
		int		NullDotDiff;
		int		NullDotOffset;
		int		NullDotMinTh;
		int		NullDotConnTh;

		// BCR - E
		////////////////////////////////////////////////////////////////
	}BCR_PARAM;

	typedef struct _stCandiPoints
	{
		int Count;
		int CountX;
		int CountY;
		int Value[MAX_CNADIDATE_AREA];
		int PosX[MAX_CNADIDATE_AREA];
		int PosY[MAX_CNADIDATE_AREA];
		int Avg[MAX_CNADIDATE_AREA];

		void Reset()
		{
			Count = 0;
			CountX = 0;
			CountY = 0;
			memset(Value, 0x00, sizeof(int) * MAX_CNADIDATE_AREA);
			memset(PosX, 0x00, sizeof(int) * MAX_CNADIDATE_AREA);
			memset(PosY, 0x00, sizeof(int) * MAX_CNADIDATE_AREA);
			memset(Avg, 0x00, sizeof(int) * MAX_CNADIDATE_AREA);
		}

		void SetData(int nId, int val, int x, int y, int avg)
		{
			Value[nId] = val;
			PosX[nId] = x;
			PosY[nId] = y;
			Avg[nId] = avg;
		}

		void AddData(int val, int x, int y, int avg)
		{
			SetData(Count, val, x, y, avg);
			Count++;
		}

	}CANDI_POINTS;
}
// �������� �޴� �˻� Parameter - E
/////////////////////////////////////////////////////////////////////////////////