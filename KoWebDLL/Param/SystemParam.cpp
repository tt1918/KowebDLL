#include "SystemParam.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "../Defines.h"

using namespace std;
SystemParam::SystemParam()
{
	Init();
}

SystemParam::~SystemParam()
{

}

void SystemParam::Init()
{
	ImageW = 8192;
	ImageH = 8192;
	Pitch = 8192;

	ImgPart=1;
	ImgPartH = ImageH / ImgPart;

	RefBright = 80;
	RefBrightBk = RefBright;

	FlatBright = 80;
	FlatBrightBk = FlatBright;

	UseBigDefect = 0;
	CamType = 1;
	CamAngle = 0;
	MaxDefect = MAX_DEFECT;
	MaxCandiDefect = 300;
	UseMark = 1;

	PerspectiveTR = 0;
	LUTL = 0;
	LUTR = 0;
	ApplyLUT2Defect = 0;
	BrightLUT = 220;
	ContrastLUT = 200;

	BDOverkill = 0;
	CandiRect = 128;
	OverlapX = 5;
	OverlapY = 5;

	PeriodLimitX = 3.0;
	PeriodLimitY = 5.0;

	UseImgFlap = false;
	UseImgMirror = false;

	NotInsAreaOffset[0] = 0.0;
	NotInsAreaOffset[1] = 0.0;
}

int SystemParam::Load(std::wstring mainFolder)
{
	// 초기화
	Init();

	int result = 0;

	wifstream ifs;
	std::wstring path = mainFolder + SYSTEM_FILE_NAME;
	ifs.open(path, ios::in);

	std::wstring data, item;
	vector<std::wstring> items;
	int masterKey = -1;

	if (ifs.is_open())
	{
		while (getline(ifs, data))
		{
			wstringstream ss(data);

			items.clear();

			while (getline(ss, item, L'='))
			{
				items.push_back(item);
			}

			if (items.size() == 0)
				continue;

			if (items.size() == 1)
			{
				if (items[0] == L"[IMAGE]")		masterKey = 0;
				else if (items[0] == L"[SYSTEM]")	masterKey = 1;
				else if (items[0] == L"[PARAM]")	masterKey = 2;
			}
			else
			{
				if (masterKey == 0)	// IMAGE
				{
					if (items[0] == L"WIDTH")					ImageW = stoi(items[1]);
					else if (items[0] == L"PITCH")				Pitch = stoi(items[1]);
					else if (items[0] == L"HEIGHT")				ImageH = stoi(items[1]);
					else if (items[0] == L"IMAGEPART")			ImgPart = stoi(items[1]);
					else if (items[0] == L"IMAGEGRABVALUE")		RefBright = stoi(items[1]);
					else if (items[0] == L"IMAGEFLATVALUE")		FlatBright = stoi(items[1]);
					else if (items[0] == L"USE_IMAGE_FLIP")		UseImgFlap = stoi(items[1]) == 0 ? false : true;
					else if (items[0] == L"USE_IMAGE_MIRROR")	UseImgMirror = stoi(items[1]) == 0 ? false : true;
					else if (items[0] == L"BIGDEFECT")			UseBigDefect = stoi(items[1]) == 0 ? false : true;

				}
				else if (masterKey == 1) // SYSTEM
				{
					if (items[0] == L"LINESCAN")						CamType = stoi(items[1]);
					else if (items[0] == L"CAMANGLE")				CamAngle = stoi(items[1]);
					else if(items[0] == L"MAXDEFECT")				MaxDefect = stoi(items[1]);
					else if (items[0] == L"MAXCANDIDEFECT")			MaxCandiDefect = stoi(items[1]);
					else if (items[0] == L"MARKINGDATA")			UseMark = stoi(items[1]);
					else if (items[0] == L"PERSPECTIVETR")			PerspectiveTR = stoi(items[1]);
					else if (items[0] == L"DEFECTALLPYLUT")			ApplyLUT2Defect = stoi(items[1]);
					else if (items[0] == L"BRIGHTLUT")				BrightLUT = stoi(items[1]);
					else if (items[0] == L"CONTRASTLUT")			ContrastLUT = stoi(items[1]);
					else if (items[0] == L"NOT_INSP_AREA_OFFSET1")	NotInsAreaOffset[0] = stod(items[1]);
					else if (items[0] == L"NOT_INSP_AREA_OFFSET2")	NotInsAreaOffset[1] = stod(items[1]);
					else if (items[0] == L"PERIOD_LIMIT_X")			PeriodLimitX = stod(items[1]);
					else if (items[0] == L"PERIOD_LIMIT_Y")			PeriodLimitY = stod(items[1]);
				}
				else if (masterKey == 2)	// PARAM
				{
					if (items[0] == L"BD_OVERKILL")					BDOverkill = stoi(items[1]);
					else if (items[0] == L"CANDI_RECT")				CandiRect = stoi(items[1]);
					else if (items[0] == L"OVERLAP_OFFSETX")		OverlapX = stoi(items[1]);
					else if (items[0] == L"OVERLAP_OFFSETY")		OverlapY = stoi(items[1]);
				}
			}
		}

		if (RefBright <= 0) RefBrightBk = 80;
		if (FlatBright <= 0) FlatBright = 80;
		if (MaxDefect > MAX_DEFECT) MaxDefect = MAX_DEFECT;

		if ((ImageW / CandiRect) * (ImageH / CandiRect) > MAX_CANDIDATE_AREA) CandiRect = 256;
		if (ImageW <= 0 || ImageH <= 0 || Pitch <= 0) result = -1;

		RefBrightBk = RefBright;
		FlatBrightBk = FlatBright;

		if (CamType == 1)
		{
			PerspectiveTR = 0;
			ImgPartH = ImageH / ImgPart;
		}
		else
		{
			if (ImgPart > 0) ImgPartH = ImageH / ImgPart;
			if (ImgPartH * ImgPart != ImageH)
			{
				result = -1;
			}
		}

		ifs.close();
		
	}
	return result;
}

void SystemParam::Save(std::wstring mainFolder)
{

}

void SystemParam::SetSystemParamType1(int width, int height, int pitch)
{
	ImageW = width;				// 이미지 정보 Width와 Pitch는 항상 같다.
	ImageH = height;			// 이미지 정보
	Pitch = pitch;				// 이미지 정보 Width와 Pitch는 항상 같다.

	ImgPart = 1;					// 이미지를 나눠서 촬용하는 경우 한 Frame에 포함되는 SubImage 수
	ImgPartH = ImageH / ImgPart;	// Sub 이미지 높이

	RefBright=80;				// 기준 Grab 평균 밝기
	RefBrightBk=80;

	FlatBright = 80;			// 평활화 했을 때 밝기
	FlatBrightBk = 80;			// 평활화 했을 때 밝기 백업

	UseImgFlap = false;			// 조각 영상 상하 반전
	UseImgMirror = false;		// 조각 영상 좌우 반전
	UseBigDefect = 0;			// 4배 압축한 이미지에서 불량 검출
	
	
	CamType = 1;				// 0 : AREA    1: Line Scan  Camera
	CamAngle = 0;				// 카메라 각도
	MaxDefect = MAX_DEFECT;		// 한 Frame에서 검출할 최대 불량수
	MaxCandiDefect = 300;		// 한 Frame에서 후보수
	UseMark = 1;				// 파라미터에 마킹 정보가 있고 그것을 사용한다면 1

	PerspectiveTR = 0;			// 투영, LR처럼 Perspective Transform 하면 1 안하면 0
	LUTL = 0;					// Perspective Transform에 사용되는 왼쪽 데이터
	LUTR = 0;					// Perspective Transform에 사용되는 오른쪽 데이터
	ApplyLUT2Defect = 0;		// 결과 이미지에 LUT 적용
	BrightLUT = 220;			// Bright LUT
	ContrastLUT = 200;			// Contrast LUT

	PeriodLimitX = 3.0;				// 주기 오차 X 거리(+/-) mm
	PeriodLimitY = 5.0;				// 주기 오차 Y 거리(+/-) mm

	NotInsAreaOffset[0] = 0;		// 차광막 옵셋 처리
	NotInsAreaOffset[1] = 0;		// 차광막 옵셋 처리

	BDOverkill = 0;					// 경계투과 또는 경계반사의 울렁거리는 과검제거하려면 1
	CandiRect = 256;				// 후보 격자 크기
	OverlapX = 5;					// Overlap 확인 용 X 거리
	OverlapY = 5;					// Overlap 확인 용 Y 거리
	IsMakePyramid;				// 피라미드 생성 여부
}