#include "SystemParam.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "../Defines.h"

using namespace std;
SystemParam::SystemParam()
{

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
	CandiRect = 256;
	OverlapX = 5;
	OverlapY = 5;

	PeriodLimitX = 3.0;
	PeriodLimitY = 5.0;

	UseImgFlap = false;
	UseImgMirror = false;

	NotInsAreaOffset[0] = 0.0;
	NotInsAreaOffset[1] = 0.0;
}

int SystemParam::Load(string mainFolder)
{
	// √ ±‚»≠
	Init();

	int result = 0;

	ifstream ifs;
	string path = mainFolder + SYSTEM_FILE_NAME;
	ifs.open(path, ios::in);

	string data, item;
	vector<string> items;
	int masterKey = -1;

	if (ifs.is_open())
	{
		while (getline(ifs, data))
		{
			stringstream ss(data);

			items.clear();

			while (getline(ss, item, '='))
			{
				items.push_back(item);
			}

			if (items.size() == 1)
			{
				if (items[0] == "IMAGE")		masterKey = 0;
				else if (items[0] == "SYSTEM")	masterKey = 1;
				else if (items[0] == "PARAM")	masterKey = 2;
			}
			else
			{
				if (masterKey == 0)	// IMAGE
				{
					if (items[0] == "WIDTH")					ImageW = stoi(items[1]);
					else if (items[0] == "PITCH")				Pitch = stoi(items[1]);
					else if (items[0] == "HEIGHT")				ImageH = stoi(items[1]);
					else if (items[0] == "IMAGEPART")			ImgPart = stoi(items[1]);
					else if (items[0] == "IMAGEGRABVALUE")		RefBright = stoi(items[1]);
					else if (items[0] == "IMAGEFLATVALUE")		FlatBright = stoi(items[1]);
					else if (items[0] == "USE_IMAGE_FLIP")		UseImgFlap = stoi(items[1]) == 0 ? false : true;
					else if (items[0] == "USE_IMAGE_MIRROR")	UseImgMirror = stoi(items[1]) == 0 ? false : true;
					else if (items[0] == "BIGDEFECT")			UseBigDefect = stoi(items[1]) == 0 ? false : true;

				}
				else if (masterKey == 1) // SYSTEM
				{
					if (items[0] == "LINESCAN")						CamType = stoi(items[1]);
					else if (items[0] == "CAMANGLE")				CamAngle = stoi(items[1]);
					else if(items[0] == "MAXDEFECT")				MaxDefect = stoi(items[1]);
					else if (items[0] == "MAXCANDIDEFECT")			MaxCandiDefect = stoi(items[1]);
					else if (items[0] == "MARKINGDATA")				UseMark = stoi(items[1]);
					else if (items[0] == "PERSPECTIVETR")			PerspectiveTR = stoi(items[1]);
					else if (items[0] == "DEFECTALLPYLUT")			ApplyLUT2Defect = stoi(items[1]);
					else if (items[0] == "BRIGHTLUT")				BrightLUT = stoi(items[1]);
					else if (items[0] == "CONTRASTLUT")				ContrastLUT = stoi(items[1]);
					else if (items[0] == "NOT_INSP_AREA_OFFSET1")	NotInsAreaOffset[0] = stod(items[1]);
					else if (items[0] == "NOT_INSP_AREA_OFFSET2")	NotInsAreaOffset[1] = stod(items[1]);
					else if (items[0] == "PERIOD_LIMIT_X")			PeriodLimitX = stod(items[1]);
					else if (items[0] == "PERIOD_LIMIT_Y")			PeriodLimitY = stod(items[1]);
				}
				else if (masterKey == 2)	// PARAM
				{
					if (items[0] == "BD_OVERKILL")					BDOverkill = stoi(items[1]);
					else if (items[0] == "CANDI_RECT")				CandiRect = stoi(items[1]);
					else if (items[0] == "OVERLAP_OFFSETX")			OverlapX = stoi(items[1]);
					else if (items[0] == "OVERLAP_OFFSETY")			OverlapY = stoi(items[1]);
				}
			}

			if (RefBright <= 0) RefBrightBk = 80;
			if (FlatBright <= 0) FlatBright = 80;
			if (MaxDefect > MAX_DEFECT) MaxDefect = MAX_DEFECT;
			if ((ImageW/CandiRect)*(ImageH/CandiRect) > MAX_CANDIDATE_AREA) MaxCandiDefect = 256;
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
				if(ImgPart>0) ImgPartH = ImageH / ImgPart;
				if (ImgPartH*ImgPart != ImageH)
				{
					result =  -1;
				}
			}
		}

		ifs.close();
		
	}
	return result;
}

void SystemParam::Save(string mainFolder)
{

}