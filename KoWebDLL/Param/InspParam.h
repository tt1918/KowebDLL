#pragma once
#include "../Defines.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 서버에서 받는 검사 Parameter - s

#define MAX_CNADIDATE_AREA	1280
namespace InspParam
{
	typedef struct _stCommonParam
	{
		bool	useRandomCycle;
		bool	useAi;
		bool	useFlatImage;
	
		int		GrabBright;		// 타겟 밝기
		int		TrgGrayDiff;	// 자동 제어 밝기 오차 영역

		int		BrightMax;
		int		BrightMin;

		int		PeriodLevel;	// 주기 불량을 1로 변경
		int		SizeMethod;		// 사이즈 검출 방식

		int		NoInspection;	// 1이면 No 검사
		int		AutoEdgeFind;	// 1이면 자동 검사
		int		NotInspArea;	// 미검 영역
		int		EdgeTh;			// 에지 검출 TH
		int		EdgeOffset;		// 에지 검출 후 에지 옵셋
		int		EdgeDir;		// 에지 방향 [0 : 왼쪽 못 씀, 1: 오른쪽 못 씀]

		int		AlgorithmType;	// 알고리즘 타입

		int		AttachPxl;		// 이웃하는 Blob붙이는 것

		int		TapeDist[3];	// 0: 테입거리, 1: 이형테입거리, 2: 보호 테입 거리
		int		TapeSkipDist;	// 테입 인식 후 스킵 거리
		int		TapeToOpticDist;	// 테입 센서 <-> 광학계 까지의 거리

		int		BrightCompensation; // 밝기 보상값
		
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
		int		ID;				// Bining Mode 변경할 인덱스
		bool	Updated;		// 바이닝 모드 데이터 업데이트 발생 시 처리
		int		CvtByParam;		// 파라미터에서 데이터 읽어왔을 때 변경 처리

		int		OldMode;		// 이전 바이닝 모드 인덱스
		bool	IsConvert;		// 바이닝 모드 변경 중인 경우

		int		InitStableTime;	// 카메라 변경 안정화 시간

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
		int		TH;						// 핀홀 TH
		int		ShadowOffset;			// 그림자 인식 거리
		int		ShadowTH;				// Pinhole에서 그림자 Size 구할 때 사용하는 TH
		double	ShadowSkipSize;			// 그림자 Skip Size( 그림자 사이즈가 이값보다 크면 Skip 한다)
		double	ShadowNotSkipSize;		// 그림자 Skip 제외 Size(원래 AreaTH로 구한 Size가 이 값보다 크면 그림자 관계업시 Skip하지 않는다.)

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
		int		Area;	// 군집 범위
		int		Count;	//  갯수
		int		TH;		// 군집 인식 밝기 TH;
		double	Size;	// 군집 사이즈

		void Init()
		{
			Area = 0;
			Count = 0;
			TH = 0;
			Size = 0.0;
		}

	}GROUP_ITEM;

	// 흑군집 or Cross 군집
	typedef struct _stGroupParam
	{
		int		TH;				// 군집 Area TH
		int		SizeMethod;		// 군집 사이즈 계산 방법 0:기존, 1: 대각선
		int		CvtLevel;		// 불량을 변경할 레벨
		
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

	// 쿠닉 검출 파라미터
	typedef struct _stCunicParam
	{
		int		InspArea;		// 쿠닉검사 Area
		double  CandiVal;		// 쿠닉 후보 최소값
		double  Std[MAX_LEVEL];		// 독립적인 쿠닉  STD
		double  Std1[MAX_LEVEL];		// 쿠닉 Value1과 짝을 이룸
		double	Value1[MAX_LEVEL];		// 쿠닉 Std1과 짝을 이룸

		void Init()
		{
			InspArea = 0;
			CandiVal = 0.0;
			memset(Std, 0x00, sizeof(double) * MAX_LEVEL);
			memset(Std1, 0x00, sizeof(double) * MAX_LEVEL);
			memset(Value1, 0x00, sizeof(double) * MAX_LEVEL);
		}

	}CUNIC_PARAM;

	// SC 파라미터
	typedef struct _stSC
	{
		int		Offset;					// 스크래치 검사 갭
		int		ScVal[MAX_LEVEL];		// 스크래치 Value

		void Init()
		{
			Offset = 0;
			memset(ScVal, 0x00, sizeof(int) * MAX_LEVEL);
		}
	}SC_PARAM;

	// Long SC 파라미터
	typedef struct _stLongSC
	{
		int		FrameSize;				// Long Scratch 검사할 frame Size
		int		MinFrameCnt;			// 불량 발생 시 최소 불량 Frame 사이즈

		int		LongScVal[MAX_LEVEL];	// Long 스크래치 Value
		
		void Init()
		{
			FrameSize = 0;
			MinFrameCnt = 0;

			memset(LongScVal, 0x00, sizeof(int) * MAX_LEVEL);
		}
	}LONG_SC_PARAM;

	typedef struct _stPress
	{
		bool	IsInsp;				// 검사 사용 유무

		int		UpTh;				// 찍힘 판정 최소 백점 TH
		int		DnTh;				// 찍힘 판정 최소 흑점 TH

		int		AreaUpTH;			// 영역 판정 TH
		int		AreaDnTH;			// 영역 판정 TH

		// Level Value
		int		Value[MAX_LEVEL];	// 밝기 차
		double	Size[MAX_LEVEL];	// 최소 사이즈

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
		int		MinVal;					// 기포 이물 최소 Value
		double	BrightDiff;				// 기포 이물 최소 밝기차

		int		Value[MAX_LEVEL];		// 기포 이물 Value
		double	Size[MAX_LEVEL];		// 기포 이물 Size
		
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

		// 검출 영역 설정
		int		SearchWidth;		// 10

									// BCR Size
		double	Width;				// 12
		double	Height;				// 13
		double	DiffW;				// 14
		double	DiffH;				// 15
									//  
		int		DotTh;				// 18	

									// BCR 밝기
		bool    ObjW;				// 20

									// 검출 영역 강제 설정
		bool	UseManualArea;		// 21
		int		AreaL;				// 22
		int		AreaR;				// 23

									// 탐색 못 했을 경우 영역 검색 처리
		bool	UseMatSize;			// 24
		int		WarningM;			// 25
		int		ErrorM;				// 26

		bool	DoSaveImage;		// 27

		// =======================================================
		// 내부 혹은 UI에서 받아오는 부분
		int		MarkingDefect;

		// Bcr 위치에 마킹 여부 설정
		bool	useMarkBCDPos;

		// 공장 타입
		int		CsvType;
		bool	useES;
		bool	useTG;
		bool	useETC;
		// =======================================================

		// 널링부 도트 체크
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
			NullDotMinTh = 10;	// 기본값만 넣음
			NullDotConnTh = 5;	// 기본값만 넣음

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
// 서버에서 받는 검사 Parameter - E
/////////////////////////////////////////////////////////////////////////////////