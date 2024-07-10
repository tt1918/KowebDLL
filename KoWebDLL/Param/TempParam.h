#pragma once

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>

#include "../Common.h"

/////////////////////////////////////////////////////////////////////////////////
// 검사에 쓰는 임시 파라미터 - S
namespace TempParam
{
	// TPW Hole 예외 처리 관련 
	typedef struct _stTPWHole
	{
		int		Check;	// 홀 찾아서 지움
		int		FindTH; // Hole 탐색 TH
		double	Period;	// Hole 주기
		int		Frame;	// Hole 검출한 Frame
		int		PosX;	// X Pixel
		int		PosY;	// Y Pixel

		float	CompactMin;	// Hole 형태에 대한 compactness 최소값
		float	CompactMax;	// Hole 형태에 대한 compactness 최대값
		float	MaxDiff;	// 설정한 Hole 크기의 최대 편차


	}TPW_HOLE;

	// 검사 시행 관련 파라미터
	typedef struct _stEnaInsp
	{
		int AlgoNo;			// 알고리즘 No
		int White;			// 백점 검사
		int Black;			// 흑점 검사. 
		int Scratch;		// 스크래치 검사
		int LongScratch;	// Long Scratcg 검사
		int LRInsp;			// 꼽침 검사
		int Group;			// 군집 결점 검사
		int Marking;		// 정코 마킹 검사
	}ENA_INSP;

	typedef struct _stPCInfo
	{
		wchar_t		Name[8];	// "COS-101" 같이 7자리 
		int			ID;			// 101 같은 3자리 수
		int			Optic;		// 907이면 9, 103이면 1, 405면 4 (어떤 서버를 쓰느냐에 따라 달라짐)
		int			FirstNo;	// COS번호
		int			SubNo;		// 907이면 7, 103이면 3, 405면 5
		int			OwnPort;	// 현재 PC의 Port 번호
	}PC_INFO;

	typedef struct _stDispInfo
	{
		double		InspTime;			// 검사 시간 msec
		double		MaxInspTime;		// 최대 검사 시간
		double		MinInspTime;		// 최소 검사 시간
		double		AvgBright;			// 평균 밝기
		double		AvgBrightForExpo;	// 평균 밝기
		double		AvgSharp;			// 샤프니스
		double		HDDSpace;			// HDD 남은 용량
		double		AvgSharpDetail[3];	// 이전 평균 Sharp
		double		AvgBrightDetail[3];	// 이전 평균 밝기
	}DISP_INFO;

	typedef struct _stKoonjipParam
	{
		int MaxCntInArea;			// 영역내 최고 갯수
		int AllInImage;				// 이미지 내 군집 후보 갯수

		int	GroupMaxCount;			// 군집 최대치
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
		int Max;			// 스크라치 나온 것중에서 최대값
		int Avg;
		int MaxLong;		// 스크라치 나온 것 중에서 최대값
		int MaxLongPos;		// 스크라치 나온 것 중에서 최대값 위치
	}SC_PARAM;

	// 투영 라인 Thread에 사용되는 변수
	typedef struct _stTooLine
	{
		int X;
		int Value;
		int Angle;
		int OverKill;
	}TOO_LINE_PARAM;

	// 정코 COS1에서 사용하는 변수
	typedef struct _stCoatParam
	{
		int Leading;
		int OldLeading;
		int NEL2NotInspAreaX1;
		int NEL2NotInspAreaX2;
		// 마크 검출
		int MarkValMax;
		int MarkValArray[100];
		int MarkValCount;
		int MarkValTh;

		double MaxStd;
		PXL_INFO_I MaxTD;
	}COAT_PARAM;

	typedef struct _stKnulling
	{
		int		Check;		// Knulling Check 했으면 1
		int		Value;		// TAC_LINE에서 Knull값 구한것
		double	Ratio;
		int		Found;		// Knulling 찾음:1, 못 찾음:0
	}KNULLING_PARAM;

	typedef struct _stPinhole
	{
		int IsGroup;			// 핀홀 군집 결점인 경우 1
		int IsLarge;			// 큰 핀홀 결점인 경우 1
		int IsShadeSkip;		// 그림자 스킵 핀홀인 경우 1
		double Size;		// 핀홀 크기
		double ShadeSize;	// 그림자 크기

	}PINHOLE;

	typedef struct _NotInsArea
	{
		bool IsNeedUpdate;  // data 받고 업데이트 필요하면 검사 시작 전 업데이트 진행 함.
		int Type;			// 검사 스킵 방식 
		double SttX;		// 서버에서 받은 미검 시작 위치
		double EndX;		// 서버에서 받은 미검 종료 위치

		int EdgeX;			// 계산에 의한 픽셀 좌표
		int NotInspArea;	// 미검 영역 위치

		//////////////////////////////////////////////////////////////////////////////
		// 에지 정보 백업 관련 처리
		int  OriNoInspect;		//1이면 검사안함
		int  OriNotInspArea;	//검사하지 않는 영역(양수면 왼쪽, 홀수면 오른쪽)
		int  OriAutoFindEdge;	// 기존 Auto Find Edge 설정
		// 에지 정보 백업 관련 처리 
		//////////////////////////////////////////////////////////////////////////////
	}NOT_INS_AREA;

	typedef struct _stBCR_Param
	{
		bool			IsMark;			// 바코드 영역 마킹 처리
		bool			IsMarkNext;		// 예약랏 바코드 영역 마킹 처리

		bool			IsFirstCheck;	// 검사 시작 후 제일 처음 
		double			FirstMissDist;	// 랏 변경 후 바코드 못 읽은 거리 

		int				PreEdge;		// 이전 프레임 에지 위치
		Common::RECT_I	Rect;			// 1차 영역 추출
		Common::RECT_I	RectFine;		// 후처리 영역 추출
		Common::RECT_I	RectForMatch;	// BCR 확인 실패 시 중심 처리용
		Common::RECT_I	RectCodeRead;	// CodeReader에 의해 획득한 중심점

		Common::POINT_I	CntPt;			// 바코드 중심점

		bool			IsSuccessRead;	// 바코드 인식 결과
		int				PatFind;		// 바코드 탐색 결과 순번
		bool			IsFirstCode;	// 최초 BCR 리딩
		int				Dir;			// BCR 리딩 방향

		bool			ForceInsert;		// BCR 강제 입력
		std::string	ForceData;		// BCR 강제 입력 데이터
		bool			ForceDir;			// BCR 강제 입력시 Dir
		bool			IsForceReading;		// BCR 강제 입력 처리 확인
		double			ForceREadingDist;	// BCR 강제 입력 거리


		std::string	ReadName;		// 인식 결과 바코드 정보
		int				PreInspFrame;		// 이전 BCR 인식된 Frame 번호
		std::string	PreReadName;		// 이전 인식 결과 바코드 정보
		double			CrtRealPos;		// 원단상의 실제 위치
		double			PreRealPos;		// 이전 BCR 원단상의 실제 위치
		double			OffsetY;			// 프레임 내에서 바코드 위치에 따른 OFFSET
		double			Scale[10];			// Bcr 길이 방향 Scale 계산		

		int				ReadOKCnt;			// BCR 인식 갯수

		double			NoReadWarning;
		double			NoReadError;

		int				ScaleIdx;			// Bcr Y scale 확인용
		int				Order;				// Bcr order 확인용
		double			PreFramePos;		// 

		char			FileName[100];		// BCR 조각 영상 이름

		int				TempNullPos;

		std::string	strLog;				// BCR 로그 저장용

		std::vector<std::string>	LoadedBcnoArr[2];	// 0 : 현재랏, 1 : 예약랏
		bool			FirstCompare;

		bool			PrevSuccessRead; // 연속 프레임간 바코드 인식 예외 처리용
		double			RollPos;

		bool			SkipFirstReadingError; // 서버로 부터 다른 BCR에서 데이터 읽은 경우에는 BCR 리딩 체크 에러 보내지 않
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
		int			Found;				// 구한 edge
		int			PrevFound[3];		// 이전에 구한 edge
		int			Type;				// 0: 안쪽, 1: 바깥쪽
		int			Dir;				// 0: 왼쪽, 1: 오른쪽
		int			X1;					// 검사 시작 영역
		int			X2;					// 검사 시작 영역
		int			EdgeX1;
		int			EdgeX2;
	}INSP_EDGE_PARAM;

	typedef struct _stImageParam
	{
		int				IsCopy;		// 이미지를 복사함.
		unsigned char*	Src;
		unsigned char*	Dst;
		int				Size;
	}IMG_PARAM;

	typedef struct _stCountParam
	{
		int			GrabDone;			// Grab 완료되면 1
		int			GrabOne;			// 1이면 Grab 1번만 수행하고 Stop
		int			GrabFrame;			// Grab한 Frame 수
		int			GrabFrameID;		// Grab한 FrameID
		int			GrabFrameSub;		// 분할 Grab 시 Sub Frame Idx
		int			FrameAfterFrame;	// Tape 신호 후 Frame 수
		int			InspFrame;			// 검사한 Frame수
		int			NGFrame;			// NG Frame 수
		int			NGCount;			// NG Count
		int			NGLevel;			// Class/Level 별 Counat

		int			SaveNumID;			// Save ID
		int			SaveCount;			// 저장 갯수
		int			SaveCountLot;		// Lot 바뀔 때까지 저장.

		double		FPS;					// 초당 Frame 수
	}CNT_PARAM;

	// 임시 검사 파라미터 
	typedef struct _stTempParam
	{
		int			PGStart;				// Program이 켜지면 1

		int			LatestResult;
		int			ChangeedExpoFrameID;	// 노출이 바뀐 프레임
		int			UsePrevImage;			// 직전 Frame

		TPW_HOLE	Hole;					// PTW Hole 처리 
		PC_INFO		PcInfo;					
		
		int			ConnectAIServer;		// 1이면 연결, 0이면 끊김. 
		int			FrameDiff;				// 결과 전송 시 Frame 차이
		int			FrameDiffSum[2];		// 디버깅 용

		int			RunInsp;				// 검사 중이면 1
		bool		SaveOneFrame;			// true 이면 프레임 저장

		// 노출 변경
		int			ApplyExposure;			// 1이면 Exposure를 적용
		int			ExpoData;				// 노출 시간(usec)
		int			PrevExpoData;			// 이전 노출 시간 데이터

		int			StartAutoExpo;			// 자동밝기 시작

		// 검사 영역
		INSP_EDGE_PARAM InspArea;

		int			FlatSuccess;			// Flat 처리 성공
		int			MakePyramidDone;		// 피라미드 이미지 생성 완료

		// Count 
		CNT_PARAM	CntInfo;

		int			ShiftDo;
		int			RecvMark;

		bool		FirstReframe;		// 프레임 오차 발생 시, 첫 1번은 시간 오차 발생안해도 프레임 수정

		// 화면 표시 데이터 
		DISP_INFO	DispInfo;	

		int			NBOImage;			// NBO 이미지는 평활화 다르게 한다.
		
		IMG_PARAM	ImgParam;

		// 군집 관련
		KOONJIP_PARAM	KoonJip;

		// 정코 마킹 검출
		double			MaxMark;

		// 라인 기포
		LINE_KIPO		LineKipo;

		// 세로선 얼룩
		int			MaxMultiVerLineVal;

		// 서버로 데이터 처리 
		int FrameInfoSendCount;
		int FrameInfoScanCountBack;
		int ImageInfoCount;
		int ImageInfoSendCount;

		int DefectExist[2];	// Red, Blue 불량이 있으면 1

		int LoadSizeX;
		int LoadSizeY;

		CYCLE_DEFECT_INFO CycleDefect;	// 주기 결점 정보

		int LotChangeSignal;
		int LotChnageFrame;
		int LotChangeFMCount;

		// Spot 검사
		int MaxGray;
		int MinGray;

		// 얼룩 검사
		PXL_INFO_D MuraMax;
		PXL_INFO_D MuraMin;

		// 쿠닉 검사
		VALUE_INFO_D Cunic;

		// LR
		PXL_INFO_I MaxLR;	// LR 최고값
		PXL_INFO_I MaxLRW;	// LRW 최고값

		int CkTape;	// Tape 신호 들어오면 1,2,3 바뀜
		int CkSplice;	// 2동,3동에서 Tape 신호
		int CkSpliceCount; // 3동에서 보내는 Tape 신호

		int DayChange;		// 바뀐 날짜 보관
		int MonthChange;	// 바뀐 달 보관

		// 자동 밝기 조정
		int AutoAdjStart;	// 자동 밝기 조정 시작
		bool EnaAutoAdj;	// 자동 밝기 조정을 일시적으로 정지

		int ErrCode;		// 1이면 밝기가 검사 범위를 벗어남

		// 투영 검사 
		int MaxLineVal;
		int MaxLineTDVal;

		// 투영 검사
		PXL_INFO_I MaxLine;
		PXL_INFO_I MaxLineTD;
		PXL_INFO_I MaxLineAngle;
		int MountainCnt[5];			// 산 갯수

		// 수직 라인 검사.
		PXL_INFO_I MaxWLine;
		PXL_INFO_I MaxBLine;

		// 스크라치 
		SC_PARAM Scratch;

		// Pattern
		int LastPatID;

		// LUT
		int MakeLUT;	// LUT를 만들었으면 1

		// Gain 변경
		int GainChangeFlag;		// 1 이면 Grab Stop 후 Gain 변경
		int SensorChangeFlag;	// 카메라 센서 모드 0,1,2 (0:4S, 1: 1S, 2: 2S)

		// 불량 영상
		int FlagNGSave;
		unsigned char* NGImage;		// 가장 최근 불량 영상
		unsigned char* NGImageLoad;	// Load한 불량 

		std::string	MyComName;
		std::string	LotName;
		std::string	NewLotName;
		
		__time64_t		LastSendTime;
		__time64_t		TimeCheckFrame;
		__time64_t		TimeCheckOldFrame;

		// 투영 라인 Thread에 사용되는 변수
		TOO_LINE_PARAM	Too[10];
		POS_1D_INFO		TooTD[25][8];
		int				SurfaceMax;

		// DAKANO
		DAKANO_PARAM	Dakano;

		// 물때 이미지 저장용
		int			NGMulTae;			// 물때 불량 발생함
		int			SaveNumIDCompImg;	// Save ID(1/8 압축)

		// 정코 COS1에서 사용하는 변수
		COAT_PARAM CoatPrm;

		// 무라 라인
		int SetBaseLine;
		int ReadyBaseLine;

		// 무라 
		int MuraMaxVal;

		// Delay 이미지 처리
		int GrabNextDelay;

		// Knulling 데이터
		KNULLING_PARAM Knulling;

		bool AutoLightCtrl;			// 서버에서 자동 조명제어시 1, 아니면 0( 밝기 정보를 빠르게 보냄)

		// 랏 변경 시 검사 스킵 처리
		bool	IsLeadingSkipInsp;
		int		PolSensorCnt[2];	// 랏 체인지 신호 들어오고 Sensor 값 카운트. 0:Off Count, 1:On Count

		bool	IsSkipPreSepa;		// 강제 SEPA LOT 전송 받으면 TRUE로 변경. 검사 진행하지 않도록 처리
		bool	ManualInsp;			// UI에서 검사 버튼을 눌렀을 때 검사하도록 처리(SEPA LOT 인 경우 검사 안되서 추가)

		NOT_INS_AREA	NotInspArea;	// 서버로 부터 받은 검사 스킵 영역 데이터

		double			DelayOffset;	// 서버에서 전송 받은 데이터 지연 거리. mm 거리로 환산하여 처리함. 


		void Reset()
		{

		}
	}TEMP_PARAM;

}
// 검사에 쓰는 임시 파라미터 - E
/////////////////////////////////////////////////////////////////////////////////