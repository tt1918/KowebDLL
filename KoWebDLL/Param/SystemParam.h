#pragma once
#include <stdio.h>
#include <iostream>

class SystemParam
{

public:
	///////////////////////////////////////////////////////////////////
	// masterKey : IMAGE - S
	int Width;						// �̹��� ���� Width�� Pitch�� �׻� ����.
	int Height;						// �̹��� ����
	int Pitch;						// �̹��� ���� Width�� Pitch�� �׻� ����.

	int ImgPart;					// �̹����� ������ �Կ��ϴ� ��� �� Frame�� ���ԵǴ� SubImage ��
	int ImgPartH;					// Sub �̹��� ����

	int RefBright;					// ���� Grab ��� ���
	int RefBrightBk;

	int FlatBright;					// ��Ȱȭ ���� �� ���
	int FlatBrightBk;				// ��Ȱȭ ���� �� ��� ���

	bool UseImgFlap;				// ���� ���� ���� ����
	bool UseImgMirror;				// ���� ���� �¿� ����
	int BigDefect;					// 4�� ������ �̹������� �ҷ� ����
	// masterKey : IMAGE - E
	///////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////
	// masterKey : SYSTEM - S
	int CamType;					// 0 : AREA    1: Line Scan  Camera
	int CamAngle;					// ī�޶� ����
	int MaxDefect;					// �� Frame���� ������ �ִ� �ҷ���
	int MaxCandiDefect;				// �� Frame���� �ĺ���
	int UseMark;					// �Ķ���Ϳ� ��ŷ ������ �ְ� �װ��� ����Ѵٸ� 1

	int PerspectiveTR;				// ����, LRó�� Perspective Transform �ϸ� 1 ���ϸ� 0
	int LUTL;						// Perspective Transform�� ���Ǵ� ���� ������
	int LUTR;						// Perspective Transform�� ���Ǵ� ������ ������
	int ApplyLUT2Defect;			// ��� �̹����� LUT ����
	int BrightLUT;					// Bright LUT
	int ContrastLUT;				// Contrast LUT
	
	double PeriodLimitX;			// �ֱ� ���� X �Ÿ�(+/-) mm
	double PeriodLimitY;			// �ֱ� ���� Y �Ÿ�(+/-) mm

	double NotInsAreaOffset[2];		// ������ �ɼ� ó��
	// masterKey : YSSTEM - E
	///////////////////////////////////////////////////////////////////
		
	///////////////////////////////////////////////////////////////////
	// masterKey : PARAM - S

	int BDOverkill;					// ������� �Ǵ� ���ݻ��� �ﷷ�Ÿ��� ���������Ϸ��� 1
	int CandiRect;					// �ĺ� ���� ũ��
	int OverlapX;					// Overlap Ȯ�� �� X �Ÿ�
	int OverlapY;					// Overlap Ȯ�� �� Y �Ÿ�

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