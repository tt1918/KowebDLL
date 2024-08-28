#pragma once

#include "Inspect\Inspect.h"
#include "Param/CosParam.h"
#include "Param/TempParam.h"
#include "Param/SystemParam.h"
#include "Inspect/InspectCOS.h"
#include "Param/MarkParam.h"

class Inspector
{
public:
	int						_NumOfProc;		// ������ Inspect Process ����

	/////////////////////////////////////////////////////////////////////
	// ��ó�� 
	SystemParam				_sysParam;
	// ��ó�� 
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// ��ó�� 
	FlatImage*				_pFlat;				// �̹��� ��Ȱȭ Ŭ����
	Profile*				_pProfile;			// �̹��� �������� Ŭ����
	FindEdge*				_pEdgeFinder;		// ���� ���� Ŭ����	
	PyramidImage*			_pPyramid;			// �Ƕ�̵� ���� ����
	PyramidImage*			_pPyramidSc;		// �Ƕ�̵� ���� ����
	// ��ó�� 
	/////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////
	// �˻�
	int					_SrcW;		// ���� ���� W
	int					_SrcH;		// ���� ���� H
	unsigned char		*_pSrcImg;	// ���� �̹��� ����

	Param**					_pParam;
	TempParam::TEMP_PARAM*	_tempParam;
	Inspect**				_pInspect;
	MARK_OPTIC*				_pMarkParam;
	// �˻�
	/////////////////////////////////////////////////////////////////////

public:
	Inspector();
	~Inspector();

private:
	void Release();

public:

	void Initialize(int numOfProc);

	// CosWeb�� �ٸ��� 1���� PC���� �������� ���μ����� ������ �� �־ �̸��� 
	// �Է��� �� �ֵ��� ��. 
	void SetParam(int procNum, std::wstring pcName, std::wstring path);

	// �˻� �̹����� �־��ش�.
	void SetImage(unsigned char* pBuf, int width, int height, int pitch);

	// �˻� ó���Ѵ�. 
	void RunWebInspect(int procNum);
};