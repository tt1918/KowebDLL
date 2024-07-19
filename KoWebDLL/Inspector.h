#pragma once

#include "Inspect\Inspect.h"
#include "Param/CosParam.h"
#include "Param/TempParam.h"
#include "Param/SystemParam.h"
#include "Inspect/InspectCOS.h"
class Inspector
{
public:
	/////////////////////////////////////////////////////////////////////
	// ��ó�� 
	SystemParam				_sysParam;
	Param					*_pParam;
	TempParam::TEMP_PARAM	_tempParam;
	// ��ó�� 
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// ��ó�� 
	FlatImage* _pFlat;				// �̹��� ��Ȱȭ Ŭ����
	Profile* _pProfile;				// �̹��� �������� Ŭ����
	FindEdge* _pEdgeFinder;			// ���� ���� Ŭ����	
	PyramidImage* _pPyramid;		// �Ƕ�̵� ���� ����
	PyramidImage* _pPyramidSc;		// �Ƕ�̵� ���� ����
	// ��ó�� 
	/////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////
	// �˻�
	int					_SrcW;		// ���� ���� W
	int					_SrcH;		// ���� ���� H
	unsigned char		*_pSrcImg;	// ���� �̹��� ����
	Inspect				*_pInspect;
	// �˻�
	/////////////////////////////////////////////////////////////////////

public:
	Inspector();
	~Inspector();

	void Initialize();

	void Initialize(std::string path);

	void Release();

	void SetImage(unsigned char* pBuf, int width, int pitch, int height);

	void RunWebInspect();

};