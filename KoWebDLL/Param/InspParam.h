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
	
		int		GrabBright;		// Ÿ�� ���
		int		TrgGrayDiff;	// �ڵ� ���� ��� ���� ����

		int		BrightMax;
		int		BrightMin;

		int		PeriodLevel;	// �ֱ� �ҷ��� 1�� ����
		int		SizeMethod;		// ������ ���� ���

		int		NoInspection;	// 1�̸� No �˻�
		int		AutoEdgeFind;	// 1�̸� �ڵ� �˻�
		int		NotInspArea;	// �̰� ����
		int		EdgeTh;			// ���� ���� TH
		int		EdgeOffset;		// ���� ���� �� ���� �ɼ�
		int		EdgeDir;		// ���� ���� [0 : ���� �� ��, 1: ������ �� ��]

		int		AlgorithmType;	// �˰��� Ÿ��

		int		AttachPxl;		// �̿��ϴ� Blob���̴� ��

		int		TapeDist[3];	// 0: ���԰Ÿ�, 1: �������԰Ÿ�, 2: ��ȣ ���� �Ÿ�
		int		TapeSkipDist;	// ���� �ν� �� ��ŵ �Ÿ�
		int		TapeToOpticDist;	// ���� ���� <-> ���а� ������ �Ÿ�

		int		BrightCompensation; // ��� ����
		
		void Init()
		{
			useRandomCycle = false;
			useAi = false;
			useFlatImage = true;

			GrabBright = 80;
			TrgGrayDiff = 5;

			BrightMax = 0;
			BrightMin = 0;
			
			PeriodLevel = 1;
			SizeMethod = 0;

			NoInspection = 0;
			AutoEdgeFind = 0;
			NotInspArea = 0;
			EdgeTh = 0;
			EdgeOffset = 0;
			EdgeDir = 0;

			AlgorithmType = 0;
			
			AttachPxl = 2;

			memset(TapeDist, 0x00, sizeof(int) * 3);
			TapeSkipDist = 0;
			TapeToOpticDist = 0;

			BrightCompensation = 0;
		}
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

		void Init()
		{
			UseCheck = false;
			Th = 0;
			Period = 0.0;
			Size = 0.0;
			RemoveTh = 0;
			ErrRatio = 25.0;
			CompactMin = 0.7f;
			CompactMax = 0.85f;
			Maxdiff = 0.4f;
		}
	}HOLE_SKIP_PARAM;

	typedef struct _stCamParam
	{
		int Exposure;
		double SttPosX;
		double SttPosY;
		double FrameLength;
		double ScaleX;
		double ScaleY;

		double ImageAngleFactoryY;
	}CAM_PARAM;

	typedef struct _stBasicInspParam
	{
		int TH;
		double Size;

		void Init()
		{
			TH = 0;
			Size = 0.0;
		}
	}BASIC_INSP_PARAM;

	typedef struct _stSpotParam
	{
		int		AreaTh;

		int		LvTH[MAX_LEVEL];
		double	LvSize[MAX_LEVEL];

		void Init()
		{
			AreaTh = 0;
			memset(LvTH, 0x00, sizeof(int) * MAX_LEVEL);
			memset(LvSize, 0x00, sizeof(double) * MAX_LEVEL);
		}
	}SPOT_PARAM;

	typedef struct _stBiningParam
	{
		int		ID;				// Bining Mode ������ �ε���
		bool	Updated;		// ���̴� ��� ������ ������Ʈ �߻� �� ó��
		int		CvtByParam;		// �Ķ���Ϳ��� ������ �о���� �� ���� ó��

		int		OldMode;		// ���� ���̴� ��� �ε���
		bool	IsConvert;		// ���̴� ��� ���� ���� ���

		int		InitStableTime;	// ī�޶� ���� ����ȭ �ð�

		void Init()
		{
			ID = 0;
			Updated = false;
			CvtByParam = 0;

			OldMode = -1;
			IsConvert = false;
			InitStableTime = 1000;
		}

	}BINING_PARAM;

	typedef struct _stPinhole
	{
		int		TH;						// ��Ȧ TH
		int		ShadowOffset;			// �׸��� �ν� �Ÿ�
		int		ShadowTH;				// Pinhole���� �׸��� Size ���� �� ����ϴ� TH
		double	ShadowSkipSize;			// �׸��� Skip Size( �׸��� ����� �̰����� ũ�� Skip �Ѵ�)
		double	ShadowNotSkipSize;		// �׸��� Skip ���� Size(���� AreaTH�� ���� Size�� �� ������ ũ�� �׸��� ������� Skip���� �ʴ´�.)

		void Init()
		{
			TH = 0;
			ShadowOffset = 0;
			ShadowTH = 0;
			ShadowSkipSize = 0.0;
			ShadowNotSkipSize = 0.0;
		}

	}PINHOLE_PARAM;

	typedef struct _stGroupItem
	{
		int		Area;	// ���� ����
		int		Count;	//  ����
		int		TH;		// ���� �ν� ��� TH;
		double	Size;	// ���� ������

		void Init()
		{
			Area = 0;
			Count = 0;
			TH = 0;
			Size = 0.0;
		}

	}GROUP_ITEM;

	// �決�� or Cross ����
	typedef struct _stGroupParam
	{
		int		TH;				// ���� Area TH
		int		SizeMethod;		// ���� ������ ��� ��� 0:����, 1: �밢��
		int		CvtLevel;		// �ҷ��� ������ ����
		
		GROUP_ITEM Base;
		GROUP_ITEM Level[MAX_LEVEL];

		void Init()
		{
			TH = 0;
			SizeMethod = 0;
			CvtLevel = 0;

			Base.Init();
			
			for (int i = 0; i < MAX_LEVEL; i++)
				Level[i].Init();
		}

	}GROUP_PARAM;

	// ��� ���� �Ķ����
	typedef struct _stCunicParam
	{
		int		InspArea;		// ��а˻� Area
		double  CandiVal;		// ��� �ĺ� �ּҰ�
		double  Std[MAX_LEVEL];		// �������� ���  STD
		double  Std1[MAX_LEVEL];		// ��� Value1�� ¦�� �̷�
		double	Value1[MAX_LEVEL];		// ��� Std1�� ¦�� �̷�

		void Init()
		{
			InspArea = 0;
			CandiVal = 0.0;
			memset(Std, 0x00, sizeof(double) * MAX_LEVEL);
			memset(Std1, 0x00, sizeof(double) * MAX_LEVEL);
			memset(Value1, 0x00, sizeof(double) * MAX_LEVEL);
		}

	}CUNIC_PARAM;

	// SC �Ķ����
	typedef struct _stSC
	{
		int		Offset;					// ��ũ��ġ �˻� ��
		int		ScVal[MAX_LEVEL];		// ��ũ��ġ Value

		void Init()
		{
			Offset = 0;
			memset(ScVal, 0x00, sizeof(int) * MAX_LEVEL);
		}
	}SC_PARAM;

	// Long SC �Ķ����
	typedef struct _stLongSC
	{
		int		FrameSize;				// Long Scratch �˻��� frame Size
		int		MinFrameCnt;			// �ҷ� �߻� �� �ּ� �ҷ� Frame ������

		int		LongScVal[MAX_LEVEL];	// Long ��ũ��ġ Value
		
		void Init()
		{
			FrameSize = 0;
			MinFrameCnt = 0;

			memset(LongScVal, 0x00, sizeof(int) * MAX_LEVEL);
		}
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

		void Init()
		{
			IsInsp = false;

			UpTh = 0;
			DnTh = 0;

			AreaUpTH = 0;
			AreaDnTH = 0;

			memset(Value, 0x00, sizeof(int) * MAX_LEVEL);
			memset(Size, 0x00, sizeof(double) * MAX_LEVEL);
		}

	}PRESS_PARAM;

	typedef struct _stKipo
	{
		int		MinVal;					// ���� �̹� �ּ� Value
		double	BrightDiff;				// ���� �̹� �ּ� �����

		int		Value[MAX_LEVEL];		// ���� �̹� Value
		double	Size[MAX_LEVEL];		// ���� �̹� Size
		
		void Init()
		{
			MinVal = 0;
			BrightDiff = 0.0;

			memset(Value, 0x00, sizeof(int) * MAX_LEVEL);
			memset(Size, 0x00, sizeof(double) * MAX_LEVEL);
		}

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
		bool	useMarkBCDPos;

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

		void Init()
		{
			SearchWidth = 0;
			
			Width = 0;
			Height = 0;
			DiffW = 0;
			DiffH = 0;

			DotTh = 0;

			ObjW = 0;

			UseManualArea = false;
			AreaL = 0;
			AreaR = 0;

			UseMatSize = false;
			WarningM = 10;
			ErrorM = 30;

			MarkingDefect = true;

			useMarkBCDPos = false;

			CsvType = 0;
			useES = useTG = useETC = 0;

			NullDotDiff = 0;
			NullDotOffset = 0;
			NullDotMinTh = 10;	// �⺻���� ����
			NullDotConnTh = 5;	// �⺻���� ����

		}

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