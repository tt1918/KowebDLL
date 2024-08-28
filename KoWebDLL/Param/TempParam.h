#pragma once

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>

#include "../Common.h"

/////////////////////////////////////////////////////////////////////////////////
// �˻翡 ���� �ӽ� �Ķ���� - S
namespace TempParam
{
	// TPW Hole ���� ó�� ���� 
	typedef struct _stTPWHole
	{
		int		Check;	// Ȧ ã�Ƽ� ����
		int		FindTH; // Hole Ž�� TH
		double	Period;	// Hole �ֱ�
		int		Frame;	// Hole ������ Frame
		int		PosX;	// X Pixel
		int		PosY;	// Y Pixel

		float	CompactMin;	// Hole ���¿� ���� compactness �ּҰ�
		float	CompactMax;	// Hole ���¿� ���� compactness �ִ밪
		float	MaxDiff;	// ������ Hole ũ���� �ִ� ����


	}TPW_HOLE;

	// �˻� ���� ���� �Ķ����
	typedef struct _stEnaInsp
	{
		int AlgoNo;			// �˰��� No
		int White;			// ���� �˻�
		int Black;			// ���� �˻�. 
		int Scratch;		// ��ũ��ġ �˻�
		int LongScratch;	// Long Scratcg �˻�
		int LRInsp;			// ��ħ �˻�
		int Group;			// ���� ���� �˻�
		int Marking;		// ���� ��ŷ �˻�
	}ENA_INSP;

	typedef struct _stPCInfo
	{
		std::wstring	Name;		// "COS-101" ���� 7�ڸ� 
		int				ID;			// 101 ���� 3�ڸ� ��
		int				Optic;		// 907�̸� 9, 103�̸� 1, 405�� 4 (� ������ �����Ŀ� ���� �޶���)
		int				FirstNo;	// COS��ȣ
		int				SubNo;		// 907�̸� 7, 103�̸� 3, 405�� 5

		void Set(std::wstring name)
		{
			Name = name;

			size_t pos = 0, pos1=0;
			std::wstring token;
			std::vector<std::wstring> values;

			while ((pos = name.find(L'-', pos1)) != -1)
			{
				token = name.substr(pos1, pos);
				values.push_back(token);
				pos1 = pos + 1;
			}

			if(pos1!=0)
			{
				token = name.substr(pos1, name.length());
				values.push_back(token);
			}

			ID = _wtoi(values[1].c_str());
			Optic = values[1].at(0) - '0';
			FirstNo = Optic;
			SubNo = values[1].at(2) - '0';

		};

	}PC_INFO;

	typedef struct _stDispInfo
	{
		double		InspTime;			// �˻� �ð� msec
		double		MaxInspTime;		// �ִ� �˻� �ð�
		double		MinInspTime;		// �ּ� �˻� �ð�
		double		AvgBright;			// ��� ���
		double		AvgBrightForExpo;	// ��� ���
		double		AvgSharp;			// �����Ͻ�
		double		HDDSpace;			// HDD ���� �뷮
		double		AvgSharpDetail[3];	// ���� ��� Sharp
		double		AvgBrightDetail[3];	// ���� ��� ���
	}DISP_INFO;

	typedef struct _stKoonjipParam
	{
		int MaxCntInArea;			// ������ �ְ� ����
		int AllInImage;				// �̹��� �� ���� �ĺ� ����

		int	GroupMaxCount;			// ���� �ִ�ġ
	}KOONJIP_PARAM;

	typedef struct _stLineKipo
	{
		int MaxVal;
		int	MaxPos;
	}LINE_KIPO;

	typedef struct _stCycleDefect
	{
		int		Default;		// 
		int		DefectFlag;		// 
		double	X;				// 
		double	Y;				// 
		double  PrevY;			// 
	}CYCLE_DEFECT;

	typedef struct _stCycleDefectInfo
	{
		int Count;
		CYCLE_DEFECT Defect[10000];
	}CYCLE_DEFECT_INFO;

	typedef struct _stPixelInfoD
	{
		int x;
		int y;
		double value;
	}PXL_INFO_D;

	typedef struct _stPixelInfoI
	{
		int x;
		int y;
		int value;
	}PXL_INFO_I;


	typedef struct _stValueInfo
	{
		double	MaxVal;
		int		Value;
	}VALUE_INFO_D;

	typedef struct _stPos1DInfo
	{
		int	Pos;
		int	Value;
	}POS_1D_INFO;

	typedef struct _stSCParam
	{
		bool IsInsp;		// �˻� ���� ����
		int Max;			// ��ũ��ġ ���� ���߿��� �ִ밪
		int Avg;
		int MaxLong;		// ��ũ��ġ ���� �� �߿��� �ִ밪
		int MaxLongPos;		// ��ũ��ġ ���� �� �߿��� �ִ밪 ��ġ
	}SC_PARAM;

	// ���� ���� Thread�� ���Ǵ� ����
	typedef struct _stTooLine
	{
		int X;
		int Value;
		int Angle;
		int OverKill;
	}TOO_LINE_PARAM;

	// ���� COS1���� ����ϴ� ����
	typedef struct _stCoatParam
	{
		int Leading;
		int OldLeading;
		int NEL2NotInspAreaX1;
		int NEL2NotInspAreaX2;
		// ��ũ ����
		int MarkValMax;
		int MarkValArray[100];
		int MarkValCount;
		int MarkValTh;

		double MaxStd;
		PXL_INFO_I MaxTD;
	}COAT_PARAM;

	typedef struct _stKnulling
	{
		int		Check;		// Knulling Check ������ 1
		int		Value;		// TAC_LINE���� Knull�� ���Ѱ�
		double	Ratio;
		int		Found;		// Knulling ã��:1, �� ã��:0
	}KNULLING_PARAM;

	typedef struct _stPinhole
	{
		int IsGroup;			// ��Ȧ ���� ������ ��� 1
		int IsLarge;			// ū ��Ȧ ������ ��� 1
		int IsShadeSkip;		// �׸��� ��ŵ ��Ȧ�� ��� 1
		double Size;		// ��Ȧ ũ��
		double ShadeSize;	// �׸��� ũ��

	}PINHOLE;

	typedef struct _NotInsArea
	{
		bool IsNeedUpdate;  // data �ް� ������Ʈ �ʿ��ϸ� �˻� ���� �� ������Ʈ ���� ��.
		int Type;			// �˻� ��ŵ ��� 
		double SttX;		// �������� ���� �̰� ���� ��ġ
		double EndX;		// �������� ���� �̰� ���� ��ġ

		int EdgeX;			// ��꿡 ���� �ȼ� ��ǥ
		int NotInspArea;	// �̰� ���� ��ġ

		//////////////////////////////////////////////////////////////////////////////
		// ���� ���� ��� ���� ó��
		int  OriNoInspect;		//1�̸� �˻����
		int  OriNotInspArea;	//�˻����� �ʴ� ����(����� ����, Ȧ���� ������)
		int  OriAutoFindEdge;	// ���� Auto Find Edge ����
		// ���� ���� ��� ���� ó�� 
		//////////////////////////////////////////////////////////////////////////////
	}NOT_INS_AREA;

	typedef struct _stBCR_Param
	{
		bool			IsMark;			// ���ڵ� ���� ��ŷ ó��
		bool			IsMarkNext;		// ����� ���ڵ� ���� ��ŷ ó��

		bool			IsFirstCheck;	// �˻� ���� �� ���� ó�� 
		double			FirstMissDist;	// �� ���� �� ���ڵ� �� ���� �Ÿ� 

		int				PreEdge;		// ���� ������ ���� ��ġ
		Common::RECT_I	Rect;			// 1�� ���� ����
		Common::RECT_I	RectFine;		// ��ó�� ���� ����
		Common::RECT_I	RectForMatch;	// BCR Ȯ�� ���� �� �߽� ó����
		Common::RECT_I	RectCodeRead;	// CodeReader�� ���� ȹ���� �߽���

		Common::POINT_I	CntPt;			// ���ڵ� �߽���

		bool			IsSuccessRead;	// ���ڵ� �ν� ���
		int				PatFind;		// ���ڵ� Ž�� ��� ����
		bool			IsFirstCode;	// ���� BCR ����
		int				Dir;			// BCR ���� ����

		bool			ForceInsert;		// BCR ���� �Է�
		std::wstring	ForceData;		// BCR ���� �Է� ������
		bool			ForceDir;			// BCR ���� �Է½� Dir
		bool			IsForceReading;		// BCR ���� �Է� ó�� Ȯ��
		double			ForceREadingDist;	// BCR ���� �Է� �Ÿ�


		std::wstring	ReadName;		// �ν� ��� ���ڵ� ����
		int				PreInspFrame;		// ���� BCR �νĵ� Frame ��ȣ
		std::wstring	PreReadName;		// ���� �ν� ��� ���ڵ� ����
		double			CrtRealPos;		// ���ܻ��� ���� ��ġ
		double			PreRealPos;		// ���� BCR ���ܻ��� ���� ��ġ
		double			OffsetY;			// ������ ������ ���ڵ� ��ġ�� ���� OFFSET
		double			Scale[10];			// Bcr ���� ���� Scale ���		

		int				ReadOKCnt;			// BCR �ν� ����

		double			NoReadWarning;
		double			NoReadError;

		int				ScaleIdx;			// Bcr Y scale Ȯ�ο�
		int				Order;				// Bcr order Ȯ�ο�
		double			PreFramePos;		// 

		char			FileName[100];		// BCR ���� ���� �̸�

		int				TempNullPos;

		std::wstring	strLog;				// BCR �α� �����

		std::vector<std::wstring>	LoadedBcnoArr[2];	// 0 : �����, 1 : �����
		bool			FirstCompare;

		bool			PrevSuccessRead; // ���� �����Ӱ� ���ڵ� �ν� ���� ó����
		double			RollPos;

		bool			SkipFirstReadingError; // ������ ���� �ٸ� BCR���� ������ ���� ��쿡�� BCR ���� üũ ���� ������ ��
	}BCR_PARAM;


	typedef struct _stScratchParam
	{
		int idxCnt;
		int pos[3][10];
	}SCRATCH_PARAM;

	typedef struct _stDakanoParam
	{
		int			LRSendFrame;
		int			LRSendFrameDiff;
		int			MapLineCnt;
		int			MapLineCntDiff;
		int			ShowMap;
		int			ShowOldMap;
		int			PortOpenCount;
		int			ESCRetry;
	}DAKANO_PARAM;

	typedef struct _stInspEdgeParam
	{
		int			Found;				// ���� edge
		int			PrevFound[3];		// ������ ���� edge
		int			Type;				// 0: ����, 1: �ٱ���
		int			Dir;				// 0: ����, 1: ������
		int			X1;					// �˻� ���� ���� (�˻� ��ŵ �ɼ� ������ ���� ����)
		int			X2;					// �˻� �Ϸ� ���� (�˻� ��ŵ �ɼ� ������ ���� ����)
		int			EdgeX1;				// ���� ���� �� ���� ���� ����
		int			EdgeX2;				// ���� ���� �� ���� ���� ����
	}INSP_EDGE_PARAM;

	typedef struct _stImageParam
	{
		int				IsCopy;		// �̹����� ������.
		unsigned char*	Src;
		unsigned char*	Dst;
		int				Size;
	}IMG_PARAM;

	typedef struct _stCountParam
	{
		int			GrabDone;			// Grab �Ϸ�Ǹ� 1
		int			GrabOne;			// 1�̸� Grab 1���� �����ϰ� Stop
		int			GrabFrame;			// Grab�� Frame ��
		int			GrabFrameID;		// Grab�� FrameID
		int			GrabFrameSub;		// ���� Grab �� Sub Frame Idx
		int			FrameAfterFrame;	// Tape ��ȣ �� Frame ��
		int			InspFrame;			// �˻��� Frame��
		int			NGFrame;			// NG Frame ��
		int			NGCount;			// NG Count
		int			NGLevel;			// Class/Level �� Counat

		int			SaveNumID;			// Save ID
		int			SaveCount;			// ���� ����
		int			SaveCountLot;		// Lot �ٲ� ������ ����.

		double		FPS;					// �ʴ� Frame ��
	}CNT_PARAM;

	typedef struct _stCropInfo
	{
		int StX;
		int width;

		int X1;
		int X2;

		void Set(double x1, double x2, double offsetX1, double offsetX2)
		{
			int stX = (int)x1;
			int edX = (int)x2;

			StX = (int)x1;

			int tmpW = ((edX - stX + 3) / 4) * 4;	// 4����Ʈ �������� �� ������ ����

			width = tmpW;

			// ���� �˻� ��ġ ó��.
			X1 = (int)(x1 + offsetX1);
			X2 = (int)(x2 - offsetX2);
		}

		// ũ�� ���� X1/X2 �����͸� �����Ѵ�.
		int GetCropX1() { return X1 - StX; }
		int GetCropX2() { return X2 - StX; }
	}CROP_INFO;


	// �ӽ� �˻� �Ķ���� 
	typedef struct _stTempParam
	{
		int				PGStart;				// Program�� ������ 1

		int				LatestResult;
		int				ChangeedExpoFrameID;	// ������ �ٲ� ������
		int				UsePrevImage;			// ���� Frame

		TPW_HOLE		Hole;					// PTW Hole ó�� 
		PC_INFO			PcInfo;					
		
		int				ConnectAIServer;		// 1�̸� ����, 0�̸� ����. 
		int				FrameDiff;				// ��� ���� �� Frame ����
		int				FrameDiffSum[2];		// ����� ��

		int				RunInsp;				// �˻� ���̸� 1
		bool			SaveOneFrame;			// true �̸� ������ ����

		// ���� ����
		int				ApplyExposure;			// 1�̸� Exposure�� ����
		int				ExpoData;				// ���� �ð�(usec)
		int				PrevExpoData;			// ���� ���� �ð� ������

		int				StartAutoExpo;			// �ڵ���� ����

		// �˻� ����
		INSP_EDGE_PARAM InspArea;

		// ��ü �̹������� �˻� �̹����� ����
		CROP_INFO		CropInfo;

		int				FlatSuccess;			// Flat ó�� ����
		int				MakePyramidDone;		// �Ƕ�̵� �̹��� ���� �Ϸ�

		// Count 
		CNT_PARAM		CntInfo;

		int				ShiftDo;
		int				RecvMark;

		int				GrabFrameID;
		bool			FirstReframe;		// ������ ���� �߻� ��, ù 1���� �ð� ���� �߻����ص� ������ ����

		// ȭ�� ǥ�� ������ 
		DISP_INFO		DispInfo;	

		int				NBOImage;			// NBO �̹����� ��Ȱȭ �ٸ��� �Ѵ�.
		
		IMG_PARAM		ImgParam;

		// ���� ����
		KOONJIP_PARAM	KoonJip;

		// ���� ��ŷ ����
		double			MaxMark;

		// ���� ����
		LINE_KIPO		LineKipo;

		// ���μ� ���
		int				MaxMultiVerLineVal;

		// ������ ������ ó�� 
		int				FrameInfoSendCount;
		int				FrameInfoScanCountBack;
		int				ImageInfoCount;
		int				ImageInfoSendCount;

		int				DefectExist[2];	// Red, Blue �ҷ��� ������ 1

		int				LoadSizeX;
		int				LoadSizeY;

		CYCLE_DEFECT_INFO CycleDefect;	// �ֱ� ���� ����

		int				LotChangeSignal;
		int				LotChnageFrame;
		int				LotChangeFMCount;

		// Spot �˻�
		int				MaxGray;
		int				MinGray;

		// ��� �˻�
		PXL_INFO_D		MuraMax;
		PXL_INFO_D		MuraMin;

		// ��� �˻�
		VALUE_INFO_D	Cunic;

		// LR
		PXL_INFO_I		MaxLR;	// LR �ְ�
		PXL_INFO_I		MaxLRW;	// LRW �ְ�

		int				CkTape;	// Tape ��ȣ ������ 1,2,3 �ٲ�
		int				CkSplice;	// 2��,3������ Tape ��ȣ
		int				CkSpliceCount; // 3������ ������ Tape ��ȣ

		int				DayChange;		// �ٲ� ��¥ ����
		int				MonthChange;	// �ٲ� �� ����

		// �ڵ� ��� ����
		int				AutoAdjStart;	// �ڵ� ��� ���� ����
		bool			EnaAutoAdj;	// �ڵ� ��� ������ �Ͻ������� ����

		int				ErrCode;		// 1�̸� ��Ⱑ �˻� ������ ���

		// ���� �˻� 
		int				MaxLineVal;
		int				MaxLineTDVal;

		// ���� �˻�
		PXL_INFO_I		MaxLine;
		PXL_INFO_I		MaxLineTD;
		PXL_INFO_I		MaxLineAngle;
		int				MountainCnt[5];			// �� ����

		// ���� ���� �˻�.
		PXL_INFO_I		MaxWLine;
		PXL_INFO_I		MaxBLine;

		// ��ũ��ġ 
		SC_PARAM		Scratch;

		// Pattern
		int				LastPatID;

		// LUT
		int				MakeLUT;	// LUT�� ��������� 1

		// Gain ����
		int				GainChangeFlag;		// 1 �̸� Grab Stop �� Gain ����
		int				SensorChangeFlag;	// ī�޶� ���� ��� 0,1,2 (0:4S, 1: 1S, 2: 2S)

		// �ҷ� ����
		int				FlagNGSave;
		unsigned char*	NGImage;		// ���� �ֱ� �ҷ� ����
		unsigned char*	NGImageLoad;	// Load�� �ҷ� 

		std::wstring	MyComName;
		std::wstring	LotName;
		std::wstring	NewLotName;
		
		__time64_t		LastSendTime;
		__time64_t		TimeCheckFrame;
		__time64_t		TimeCheckOldFrame;

		// ���� ���� Thread�� ���Ǵ� ����
		TOO_LINE_PARAM	Too[10];
		POS_1D_INFO		TooTD[25][8];
		int				SurfaceMax;

		// DAKANO
		DAKANO_PARAM	Dakano;

		// ���� �̹��� �����
		int				NGMulTae;			// ���� �ҷ� �߻���
		int				SaveNumIDCompImg;	// Save ID(1/8 ����)

		// ���� COS1���� ����ϴ� ����
		COAT_PARAM		CoatPrm;

		// ���� ����
		int				SetBaseLine;
		int				ReadyBaseLine;

		// ���� 
		int				MuraMaxVal;

		// Delay �̹��� ó��
		int				GrabNextDelay;

		// Knulling ������
		KNULLING_PARAM	Knulling;

		bool			AutoLightCtrl;			// �������� �ڵ� ��������� 1, �ƴϸ� 0( ��� ������ ������ ����)

		// �� ���� �� �˻� ��ŵ ó��
		bool			IsLeadingSkipInsp;
		int				PolSensorCnt[2];	// �� ü���� ��ȣ ������ Sensor �� ī��Ʈ. 0:Off Count, 1:On Count

		bool			IsSkipPreSepa;		// ���� SEPA LOT ���� ������ TRUE�� ����. �˻� �������� �ʵ��� ó��
		bool			ManualInsp;			// UI���� �˻� ��ư�� ������ �� �˻��ϵ��� ó��(SEPA LOT �� ��� �˻� �ȵǼ� �߰�)

		NOT_INS_AREA	NotInspArea;	// ������ ���� ���� �˻� ��ŵ ���� ������

		double			DelayOffset;	// �������� ���� ���� ������ ���� �Ÿ�. mm �Ÿ��� ȯ���Ͽ� ó����. 


		// ��Ȧ ���� �˻� 
		PINHOLE			Pinhole;

		_stTempParam()
		{

		}

		~_stTempParam()
		{

		}

		void Reset()
		{

		}
	}TEMP_PARAM;

}
// �˻翡 ���� �ӽ� �Ķ���� - E
/////////////////////////////////////////////////////////////////////////////////