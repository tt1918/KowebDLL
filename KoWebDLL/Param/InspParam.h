#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 서버에서 받는 검사 Parameter - s
namespace InspParam
{
	typedef struct _stCommonParam
	{
		bool	useRandomCycle;
		bool	useAi;

		int		TrgGrayDiff;	// 자동 제어 밝기 오차 영역
		int		MakePyramid;	// 

		int		BrightMax;
		int		BrightMin;

		int		AreaUpTh;
		int		AreaDnTh;

		int		PeriodLevel;	// 주기 불량을 1로 변경
		int		SizeMethod;		// 사이즈 검출 방식

		int		NoInspection;	// No 검사
		int		EdgeTh;			// 에지 검출 TH
		int		EdgeOffset;		// 에지 검출 후 에지 옵셋

		int		AlgorithmType;	// 알고리즘 타입

		int		TapeDist[3];	// 0: 테입거리, 1: 이형테입거리, 2: 보호 테입 거리
		int		TapeSkipDist;	// 테입 인식 후 스킵 거리
		int		TapeToOpticDist;	// 테입 센서 <-> 광학계 까지의 거리

		int		BrightCompensation; // 밝기 보상값
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
		int		ID;				// Bining Mode 변경할 인덱스
		bool	Updated;		// 바이닝 모드 데이터 업데이트 발생 시 처리
		int		CvtByParam;		// 파라미터에서 데이터 읽어왔을 때 변경 처리

		int		OldMode;		// 이전 바이닝 모드 인덱스
		bool	IsConvert;		// 바이닝 모드 변경 중인 경우

		int		InitStableTime;	// 카메라 변경 안정화 시간
	}BINING_PARAM;

	typedef struct _stPinhole
	{
		int TH;						// 핀홀 TH
		int ShadowOffset;			// 그림자 인식 거리
		int ShadowTH;				// Pinhole에서 그림자 Size 구할 때 사용하는 TH
		double ShadowSkipSize;		// 그림자 Skip Size( 그림자 사이즈가 이값보다 크면 Skip 한다)
		double ShadowNotSkipSize;	// 그림자 Skip 제외 Size(원래 AreaTH로 구한 Size가 이 값보다 크면 그림자 관계업시 Skip하지 않는다.)

	}PINHOLE_PARAM;

	typedef struct _stGroupItem
	{
		int		Area;	// 군집 범위
		int		Count;	//  갯수
		int		Value;	// 군집 인식 발기 Value;
		double	Size;	// 군집 사이즈
	}GROUP_ITEM;

	// 흑군집 or Cross 군집
	typedef struct _stGroupParam
	{
		int		TH;				// 군집 Area TH
		int		SizeMethod;		// 군집 사이즈 계산 방법 0:기존, 1: 대각선
		int		CvtLevel;		// 불량을 변경할 레벨
		
		GROUP_ITEM Base;
		GROUP_ITEM Level[10];

	}GROUP_PARAM;

	// 쿠닉 검출 파라미터
	typedef struct _stCunicParam
	{
		int		InspArea;		// 쿠닉검사 Area
		double  CandiVal;		// 쿠닉 후보 최소값
		double  Std[10];		// 독립적인 쿠닉  STD
		double  Std1[10];		// 쿠닉 Value1과 짝을 이룸
		double	Value1[10];		// 쿠닉 Std1과 짝을 이룸
	}CUNIC_PARAM;

	// SC 파라미터
	typedef struct _stSC
	{
		int		ScVal[10];		// 스크래치 Value
	}SC_PARAM;

	// Long SC 파라미터
	typedef struct _stLongSC
	{
		int		FrameSize;		// Long Scratch 검사할 frame Size
		int		MinFrameCnt;	// 불량 발생 시 최소 불량 Frame 사이즈

		int		LongScVal[10];	// Long 스크래치 Value
	}LONG_SC_PARAM;

	typedef struct _stPress
	{
		int		UpTh;			// 찍힘 판정 최소 백점 TH
		int		DnTh;			// 찍힘 판정 최소 흑점 TH

		int		AreaUpTH;		// 영역 판정 TH
		int		AreaDnTH;		// 영역 판정 TH

		// Level Value
		int		Value[10];		// 밝기 차
		double	Size[10];		// 최소 사이즈
	}PRESS_PARAM;


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
		int		ErrorM;			// 26

		bool	DoSaveImage;		// 27

				// =======================================================
				// 내부 혹은 UI에서 받아오는 부분
		int		MarkingDefect;

		// Bcr 위치에 마킹 여부 설정
		int		BCDPos;

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
	}BCR_PARAM;
}
// 서버에서 받는 검사 Parameter - E
/////////////////////////////////////////////////////////////////////////////////