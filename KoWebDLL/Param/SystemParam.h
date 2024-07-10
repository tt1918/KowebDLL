#pragma once
#include <stdio.h>
#include <iostream>

class SystemParam
{

public:
	///////////////////////////////////////////////////////////////////
	// masterKey : IMAGE - S
	int Width;						// 이미지 정보 Width와 Pitch는 항상 같다.
	int Height;						// 이미지 정보
	int Pitch;						// 이미지 정보 Width와 Pitch는 항상 같다.

	int ImgPart;					// 이미지를 나눠서 촬용하는 경우 한 Frame에 포함되는 SubImage 수
	int ImgPartH;					// Sub 이미지 높이

	int RefBright;					// 기준 Grab 평균 밝기
	int RefBrightBk;

	int FlatBright;					// 평활화 했을 때 밝기
	int FlatBrightBk;				// 평활화 했을 때 밝기 백업

	bool UseImgFlap;				// 조각 영상 상하 반전
	bool UseImgMirror;				// 조각 영상 좌우 반전
	int BigDefect;					// 4배 압축한 이미지에서 불량 검출
	// masterKey : IMAGE - E
	///////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////
	// masterKey : SYSTEM - S
	int CamType;					// 0 : AREA    1: Line Scan  Camera
	int CamAngle;					// 카메라 각도
	int MaxDefect;					// 한 Frame에서 검출할 최대 불량수
	int MaxCandiDefect;				// 한 Frame에서 후보수
	int UseMark;					// 파라미터에 마킹 정보가 있고 그것을 사용한다면 1

	int PerspectiveTR;				// 투영, LR처럼 Perspective Transform 하면 1 안하면 0
	int LUTL;						// Perspective Transform에 사용되는 왼쪽 데이터
	int LUTR;						// Perspective Transform에 사용되는 오른쪽 데이터
	int ApplyLUT2Defect;			// 결과 이미지에 LUT 적용
	int BrightLUT;					// Bright LUT
	int ContrastLUT;				// Contrast LUT
	
	double PeriodLimitX;			// 주기 오차 X 거리(+/-) mm
	double PeriodLimitY;			// 주기 오차 Y 거리(+/-) mm

	double NotInsAreaOffset[2];		// 차광막 옵셋 처리
	// masterKey : YSSTEM - E
	///////////////////////////////////////////////////////////////////
		
	///////////////////////////////////////////////////////////////////
	// masterKey : PARAM - S

	int BDOverkill;					// 경계투과 또는 경계반사의 울렁거리는 과검제거하려면 1
	int CandiRect;					// 후보 격자 크기
	int OverlapX;					// Overlap 확인 용 X 거리
	int OverlapY;					// Overlap 확인 용 Y 거리

	// masterKey : PARAM - E
	///////////////////////////////////////////////////////////////////
	

public:
	SystemParam();
	~SystemParam();

	int Load(std::string mainFolder);
	void Save(std::string mainFolder);

private:

	void Init();
};