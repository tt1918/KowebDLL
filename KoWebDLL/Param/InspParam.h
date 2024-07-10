#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������� �޴� �˻� Parameter - s
namespace InspParam
{
	typedef struct _stCommonParam
	{
		bool	useRandomCycle;
		bool	useAi;

		int		TrgGrayDiff;	// �ڵ� ���� ��� ���� ����
		int		MakePyramid;	// 

		int		BrightMax;
		int		BrightMin;

		int		AreaUpTh;
		int		AreaDnTh;

		int		PeriodLevel;	// �ֱ� �ҷ��� 1�� ����
		int		SizeMethod;		// ������ ���� ���

		int		NoInspection;	// No �˻�
		int		EdgeTh;			// ���� ���� TH
		int		EdgeOffset;		// ���� ���� �� ���� �ɼ�

		int		AlgorithmType;	// �˰��� Ÿ��

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
		BASIC_INSP_PARAM Level[10];
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
		int		Value;	// ���� �ν� �߱� Value;
		double	Size;	// ���� ������
	}GROUP_ITEM;

	// �決�� or Cross ����
	typedef struct _stGroupParam
	{
		int		TH;				// ���� Area TH
		int		SizeMethod;		// ���� ������ ��� ��� 0:����, 1: �밢��
		int		CvtLevel;		// �ҷ��� ������ ����
		
		GROUP_ITEM Base;
		GROUP_ITEM Level[10];

	}GROUP_PARAM;

	// ��� ���� �Ķ����
	typedef struct _stCunicParam
	{
		int		InspArea;		// ��а˻� Area
		double  CandiVal;		// ��� �ĺ� �ּҰ�
		double  Std[10];		// �������� ���  STD
		double  Std1[10];		// ��� Value1�� ¦�� �̷�
		double	Value1[10];		// ��� Std1�� ¦�� �̷�
	}CUNIC_PARAM;

	// SC �Ķ����
	typedef struct _stSC
	{
		int		ScVal[10];		// ��ũ��ġ Value
	}SC_PARAM;

	// Long SC �Ķ����
	typedef struct _stLongSC
	{
		int		FrameSize;		// Long Scratch �˻��� frame Size
		int		MinFrameCnt;	// �ҷ� �߻� �� �ּ� �ҷ� Frame ������

		int		LongScVal[10];	// Long ��ũ��ġ Value
	}LONG_SC_PARAM;

	typedef struct _stPress
	{
		int		UpTh;			// ���� ���� �ּ� ���� TH
		int		DnTh;			// ���� ���� �ּ� ���� TH

		int		AreaUpTH;		// ���� ���� TH
		int		AreaDnTH;		// ���� ���� TH

		// Level Value
		int		Value[10];		// ��� ��
		double	Size[10];		// �ּ� ������
	}PRESS_PARAM;


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
		int		ErrorM;			// 26

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
}
// �������� �޴� �˻� Parameter - E
/////////////////////////////////////////////////////////////////////////////////